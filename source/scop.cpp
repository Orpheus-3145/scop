#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h> 

#include "scop.hpp"
#include "parser.hpp"
#include "exception.hpp"
#include "math/quaternion.hpp"
#include "math/utilities.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



GraphicGL::GraphicGL( GLuint shader, std::string const& uniformName ) {
	this->_shader = shader;
	this->_transformation = idMat();

	this->_shaderLocation = glGetUniformLocation(this->_shader, uniformName.data());
	if (this->_shaderLocation == -1)
		throw OpenGlException("Uniform variable not found in shader: " + uniformName);
}

void GraphicGL::updateShader( void ) {
	Matrix4 dataToUniform;
	if (SCOP_COLUMN_MAJOR)
		dataToUniform = this->_transformation.transpose();
	else
		dataToUniform = this->_transformation;

	glUniformMatrix4fv(this->_shaderLocation, 1, GL_FALSE, dataToUniform.data());
}

void GraphicGL::reset( void ) noexcept {
	this->_transformation = idMat();
}


void ModelGL::rotate( float pitch, float yaw, float roll ) noexcept {
	pitch = toRadiants(pitch / 2.0f);	// vertical rotation: cameraLeft is the axis
	yaw = toRadiants(yaw / 2.0f);		// horizontal rotation: up is the axis
	roll = toRadiants(roll / 2.0f);		// frontal rotation: z is the axis

	Quatern qPitch(pitch, VectF3{-1.0f, 0.0f, 0.0f});
	Quatern qYaw(-yaw, VectF3{0.0f, 1.0f, 0.0f});
	Quatern qRoll(roll, VectF3{0.0f, 0.0f, 1.0f});

	Quatern q = qPitch * qYaw * qRoll;		// rotation order: roll -> yaw -> pitch
	this->_transformation *= q.getMatrix();
	GraphicGL::updateShader();
}

void ModelGL::translate( VectF3 const& trans ) noexcept {
	this->_transformation *= transMat(trans);
	GraphicGL::updateShader();
}

void ModelGL::scale( VectF3 const& scale ) noexcept {
	this->_transformation *= scaleMat(scale);
	GraphicGL::updateShader();
}


void CameraGL::moveForward( float delta ) noexcept {
	this->_position -= this->_cameraForward * delta;
	this->updateShader();
}

void CameraGL::moveBackward( float delta ) noexcept {
	this->_position += this->_cameraForward * delta;
	this->updateShader();
}

void CameraGL::moveRight( float delta ) noexcept {
	this->_position += this->_cameraLeft * delta;
	this->updateShader();
}

void CameraGL::moveLeft( float delta ) noexcept {
	this->_position -= this->_cameraLeft * delta;
	this->updateShader();
}

void CameraGL::rotate( float pitch, float yaw, float roll ) noexcept {
	pitch = toRadiants(pitch / 2.0f);	// vertical rotation: cameraLeft is the axis
	yaw = toRadiants(yaw / 2.0f);		// horizontal rotation: up is the axis
	roll = toRadiants(roll / 2.0f);		// frontal rotation: z is the axis

	Quatern qPitch(pitch, this->_cameraLeft);
	Quatern qYaw(-yaw, this->__up);
	Quatern qRoll(roll, this->_forward);

	Quatern q = qPitch * qYaw * qRoll;		// rotation order: roll -> yaw -> pitch
	Quatern qForward{0.0f, this->_forward.x, this->_forward.y, this->_forward.z};
	Quatern qForwardRotated = (q * qForward) * q.conjugate();
	this->_forward = qForwardRotated.vector();

	if (roll != 0.0f) {		// with a roll rotation the world up has to be changed
		Quatern qUp{0.0f, this->__up.x, this->__up.y, this->__up.z};
		Quatern qUpRotated = (q * qUp) * q.conjugate();
		this->__up = qUpRotated.vector();
	}
	this->updateShader();
}

void CameraGL::updateShader( void ) {
	VectF3 cameraTarget = this->_position + this->_forward;		// position that the camera is watching
	this->_cameraForward = normalize(this->_position - cameraTarget);
	this->_cameraLeft = normalize(this->__up ^ this->_cameraForward );
	this->_cameraUp = this->_cameraForward ^ this->_cameraLeft;

	Matrix4 rotation{std::array<float,16>{
		this->_cameraLeft.x,     this->_cameraLeft.y,     this->_cameraLeft.z,     0.0f,
		this->_cameraUp.x,       this->_cameraUp.y,       this->_cameraUp.z,       0.0f,
		this->_cameraForward.x,  this->_cameraForward.y,  this->_cameraForward.z,  0.0f,
		0.0f,                    0.0f,                    0.0f,                    1.0f
	}};
	Matrix4 translation = transMat(this->_position * -1);

	this->_transformation = rotation * translation;
	GraphicGL::updateShader();
}

void ProjectionGL::setAspect( uint32_t width, uint32_t height ) noexcept {
	this->_aspect = static_cast<float>(width) / static_cast<float>(height);
	this->updateShader();
}

void ProjectionGL::updateShader( void ) {
	float fov = SCOP_FOV;
	float near = SCOP_NEAR;
	float far = SCOP_FAR;
	if ((fov < -M_PI * 2) or (fov > M_PI * 2))
		fov = toRadiants(fov);
	float f = 1.0f / tanf(fov / 2.0f);

	if (SCOP_PRJ_FINITE == true)		// use finite projection
		this->_transformation = Matrix4(std::array<float,16>{
			f / this->_aspect,  .0f,  .0f,                               .0f,
			.0f,                f,    .0f,                               .0f,
			.0f,                .0f,  -1 * (far + near) / (far - near),  -2 * far * near / (far - near),
			.0f,                .0f,  -1.0f,                             .0f
		});
	else			// use infinte projection, far goes to inf
		this->_transformation = Matrix4(std::array<float,16>{
			f / this->_aspect,  .0f,  .0f,        .0f,
			.0f,                f,    .0f,        .0f,
			.0f,                .0f,  -1,         -2 * near,
			.0f,                .0f,  -1.0f,      .0f
		});
	GraphicGL::updateShader();
}


ScopGL::~ScopGL( void ) noexcept {
	if (this->_VBO)
		glDeleteVertexArrays(1, &this->_VBO);
	if (this->_VAO)
		glDeleteBuffers(1, &this->_VAO);
	if (this->_EBO)
		glDeleteBuffers(1, &this->_EBO);
	if (this->_shaderProgram)
		glDeleteProgram(this->_shaderProgram);
	if (this->_window)
		glfwDestroyWindow(this->_window);
	glfwTerminate();
}

void ScopGL::parseFile( std::string const& fileName ) {
	FileParser parser;
	ParsedData data = parser.parse(fileName);

	data.triangolate();
	data.fixTrianglesOrientation();
	data.fillTexturesAndNormals();
	data.fillBuffers();
	this->_VBOdata = data.getVBO();
	if (data.hasFaces())
		this->_EBOdata = data.getEBO();
	std::cout << "parsed file " << fileName << std::endl;
}

void ScopGL::createWindow( int32_t width, int32_t height ) {
	if (this->_window)
		throw AppException("Window already initialized");
	else if (width < 0 or height < 0)
		throw AppException("Negative value for width or height");
	else if (!glfwInit()) {
		const char* description;
		glfwGetError(&description);
		throw GlfwException("Initialization of glfw failed: " + std::string(description));
	}
	std::cout << "glfw initialized, version: " << GLFW_CONTEXT_VERSION_MAJOR << "." << GLFW_CONTEXT_VERSION_MINOR << "." << GLFW_VERSION_REVISION << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	this->_window = glfwCreateWindow(width, height, "SCOP", nullptr, nullptr);
	if (!this->_window) {
		const char* description;
		glfwGetError(&description);
		throw GlfwException("Creation of window failed: " + std::string(description));
	}
	std::cout << "created window " << width << "x" << height << "p" << std::endl;

	// save a reference of Scop inside GLFW
	glfwSetWindowUserPointer(this->_window, this);

	// center the window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int32_t posX = (mode->width - width) / 2;
	int32_t posY = (mode->height - height) / 2;
	glfwSetWindowPos(this->_window, posX, posY);

	glfwMakeContextCurrent(this->_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw GlfwException("Initialization of GLAD failed");
	std::cout << "using GLAD" << std::endl;
	this->_widthWindow = static_cast<uint32_t>(width);
	this->_heightWindow = static_cast<uint32_t>(height);
}

void ScopGL::initGL( std::string const& vertexShaderSource, std::string const& textureShaderSource, std::string const& textureFile ) {
	if (!this->_window)
		throw AppException("GLFW not started, call .createWindow()");
	else if (this->_shaderProgram)
		throw AppException("Setup openGL already done");

	glViewport(0, 0, this->_widthWindow, this->_heightWindow);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "OpenGL initialized version: " << major << "." << minor << std::endl;
	std::cout << "full: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	this->_createShader(GL_VERTEX_SHADER, vertexShaderSource);
	std::cout << "loaded vertex shader: " << vertexShaderSource << std::endl;

	this->_createShader(GL_FRAGMENT_SHADER, textureShaderSource);
	std::cout << "loaded fragment shader: " << textureShaderSource << std::endl;

	this->_loadTexture(textureFile);
	std::cout << "loaded texture: " << textureFile << std::endl;

	this->_loadBuffersInGPU();
	std::cout << "VBO uploaded to GPU" << std::endl;

	this->_model = std::make_unique<ModelGL>(this->_shaderProgram);
	this->_camera = std::make_unique<CameraGL>(this->_shaderProgram, VectF3{0.0f, 0.0f, SCOP_CAMERA_DISTANCE});
	this->_projection = std::make_unique<ProjectionGL>(this->_shaderProgram, this->_widthWindow, this->_heightWindow);
}

void ScopGL::loop( void ) {
	if (!this->_window)
		throw AppException("GLFW not started, call .createWindow()");
	else if (!this->_VBOdata)
		throw AppException("Data not parsed, call .parseFile()");
	else if (!this->_shaderProgram)
		throw AppException("OpenGL not started, call .initGL()");

	glUseProgram(this->_shaderProgram);
	this->_model->updateShader();
	this->_camera->updateShader();
	this->_projection->updateShader();

	this->_setupCallbacks();
	std::cout << "setup callbacks" << std::endl;

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	this->_centerCursor();
	std::cout << "starting loop" << std::endl;
	while (!glfwWindowShouldClose(this->_window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, this->_texture);
		glBindVertexArray(this->_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);

		this->_moveCamera();

		if (this->_EBO)
			glDrawElements(GL_TRIANGLES, this->_EBOdata->size, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, this->_VBOdata->size);

		glfwSwapBuffers(this->_window);
		glfwPollEvents();
	}
}

void ScopGL::resetCanvaSize( uint32_t width, uint32_t height ) {
	if (!this->_shaderProgram)
		throw AppException("OpenGL not started, call .initGL()");

	glViewport(0, 0, width, height);
	this->_projection->setAspect(width, height);
	this->_widthWindow = width;
	this->_heightWindow = height;
	this->_centerCursor();
}

void ScopGL::closeWindow( void ) {
	if (!this->_window)
		throw AppException("Window not created, call .createWindow()");

	glfwSetWindowShouldClose(this->_window, GLFW_TRUE);
}

void ScopGL::_createShader( GLenum type, std::string const& shaderFile ) {
	if (!this->_shaderProgram)
		this->_shaderProgram = glCreateProgram();

	uint32_t shader = this->_loadShader(type, shaderFile);
	glAttachShader(this->_shaderProgram, shader);
	glDeleteShader(shader);
	glLinkProgram(this->_shaderProgram);

	// check linking status
	int32_t  success;
	char infoLog[512];
	glGetProgramiv(this->_shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(this->_shaderProgram, 512, NULL, infoLog);
		throw OpenGlException("Failed to link shaders: " + std::string(infoLog));
	}
}

uint32_t ScopGL::_loadShader( GLenum type, std::string const& fileName ) {
	std::ifstream readFile;

	readFile.open(fileName, std::ifstream::in);
	if (!readFile)
		throw OpenGlException("Failed to open shader file: " + fileName);

	std::stringstream buffer;
	buffer << readFile.rdbuf();
	readFile.close();

	std::string fileContentStr = buffer.str();
	const char* fileContent = fileContentStr.c_str();
	uint32_t shaderRef = glCreateShader(type);
	glShaderSource(shaderRef, 1, &fileContent, NULL);
	glCompileShader(shaderRef);

	// check compilation status
	int32_t  success;
	char infoLog[512];
	glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shaderRef, 512, NULL, infoLog);
		throw OpenGlException("Failed to compile shader: " + fileName + ", trace: " + infoLog);
	}
	return shaderRef;
}

void ScopGL::_loadTexture( std::string const& texturePath ) {
	glGenTextures(1, &this->_texture);
	glBindTexture(GL_TEXTURE_2D, this->_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int32_t width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
		throw OpenGlException("Failed to load texture in: " + texturePath);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(data);
}

void ScopGL::_setupCallbacks( void ) {
	glfwSetFramebufferSizeCallback(this->_window, [](GLFWwindow* window, int32_t w, int32_t h) {
		ScopGL* self = static_cast<ScopGL*>(glfwGetWindowUserPointer(window));
		if (self)
			self->resetCanvaSize(w, h);
	});

	glfwSetKeyCallback(this->_window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
		(void) scancode; (void) mods;
		ScopGL* self = static_cast<ScopGL*>(glfwGetWindowUserPointer(window));
		if (!self)
			return;

		if (key == GLFW_KEY_T and action == GLFW_PRESS)
			self->_toggleTextures();
		else if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS)
			self->closeWindow();
	});

	glfwSetWindowFocusCallback(this->_window, [](GLFWwindow* window, int32_t focused) {
		if (focused == GLFW_TRUE) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			ScopGL* self = static_cast<ScopGL*>(glfwGetWindowUserPointer(window));
			if (self)
				self->_centerCursor();
		} else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	});

	glfwSetCursorPosCallback(this->_window, [](GLFWwindow* window, double posX, double posY) {
		if (glfwGetWindowAttrib(window, GLFW_FOCUSED) == false)
			return;
		ScopGL* self = static_cast<ScopGL*>(glfwGetWindowUserPointer(window));
		if (self)
			self->_rotateCamera(posX, posY);
	});
}

void ScopGL::_loadBuffersInGPU( void ) {
	if (!this->_window)
		throw AppException("GLFW not started, call .createWindow()");
	else if (!this->_VBOdata)
		throw AppException("Data not parsed, call .parseFile()");
	else if (this->_VBO != 0U)
		throw AppException("Buffers already sent to GPU");

	glGenBuffers(1, &this->_VBO);
	if (this->_EBOdata)
		glGenBuffers(1, &this->_EBO);
	glGenVertexArrays(1, &this->_VAO);
	glBindVertexArray(this->_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);

	// load vertex data
	glBufferData(GL_ARRAY_BUFFER, this->_VBOdata->size * this->_VBOdata->stride, this->_VBOdata->getData(), GL_STATIC_DRAW);
	// vertex metadata in VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, this->_VBOdata->stride, (void*)0);
	glEnableVertexAttribArray(0);
	// texture metadata in VAO
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, this->_VBOdata->stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normals metadata in VAO
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, this->_VBOdata->stride, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// color metadata in VAO
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, this->_VBOdata->stride, (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (this->_EBOdata) {
		// load face indexes data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_EBOdata->size * this->_EBOdata->stride, this->_EBOdata->getData(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
}

void ScopGL::_moveCamera( void ) {
	if (!this->_camera)
		throw AppException("OpenGL not running, call .loop()");
	else if (glfwGetWindowAttrib(this->_window, GLFW_FOCUSED) == false)
		return;

	static float lastFrame = 0.0f;
	float currentFrame = glfwGetTime();
	float deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(this->_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS or
		glfwGetKey(this->_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
	{
		if (glfwGetKey(this->_window, GLFW_KEY_A) == GLFW_PRESS)
			this->_camera->rotate(0.0f, 0.0f, deltaTime * SCOP_CAMERA_SPEED);
		else if (glfwGetKey(this->_window, GLFW_KEY_D) == GLFW_PRESS)
			this->_camera->rotate(0.0f, 0.0f, -deltaTime * SCOP_CAMERA_SPEED);
		else if (glfwGetKey(this->_window, GLFW_KEY_LEFT) == GLFW_PRESS)
			this->_model->rotate(0.0f, 0.0f, deltaTime * SCOP_MODEL_ROT_SPEED);
		else if (glfwGetKey(this->_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			this->_model->rotate(0.0f, 0.0f, -deltaTime * SCOP_MODEL_ROT_SPEED);
	}
	else if (glfwGetKey(this->_window, GLFW_KEY_W) == GLFW_PRESS)
		this->_camera->moveForward(deltaTime * SCOP_CAMERA_SPEED);
	else if (glfwGetKey(this->_window, GLFW_KEY_S) == GLFW_PRESS)
		this->_camera->moveBackward(deltaTime * SCOP_CAMERA_SPEED);
	else if (glfwGetKey(this->_window, GLFW_KEY_A) == GLFW_PRESS)
		this->_camera->moveLeft(deltaTime * SCOP_CAMERA_SPEED);
	else if (glfwGetKey(this->_window, GLFW_KEY_D) == GLFW_PRESS)
		this->_camera->moveRight(deltaTime * SCOP_CAMERA_SPEED);
	else if (glfwGetKey(this->_window, GLFW_KEY_UP) == GLFW_PRESS)
		this->_model->rotate(deltaTime * SCOP_MODEL_ROT_SPEED, 0.0f, 0.0f);
	else if (glfwGetKey(this->_window, GLFW_KEY_DOWN) == GLFW_PRESS)
		this->_model->rotate(-deltaTime * SCOP_MODEL_ROT_SPEED, 0.0f, 0.0f);
	else if (glfwGetKey(this->_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		this->_model->rotate(0.0f, deltaTime * SCOP_MODEL_ROT_SPEED, 0.0f);
	else if (glfwGetKey(this->_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		this->_model->rotate(0.0f, -deltaTime * SCOP_MODEL_ROT_SPEED, 0.0f);
}

void ScopGL::_centerCursor( void ) {
	if (!this->_window)
		throw AppException("Window not created, call .createWindow()");

	this->_currCursorX = this->_widthWindow / 2.0;
	this->_currCursorY = this->_heightWindow / 2.0;
	glfwSetCursorPos(this->_window, this->_currCursorX, this->_currCursorY);
}

void ScopGL::_toggleTextures( void ) {
	if (!this->_shaderProgram)
		throw AppException("OpenGL not started, call .initGL()");

	this->_applyTextures = !this->_applyTextures;
	GLboolean applyTexture = glGetUniformLocation(this->_shaderProgram, "applyTexture");
	glUniform1i(applyTexture, this->_applyTextures);
}

void ScopGL::_rotateCamera( float posX, float posY ) {
	if (!this->_camera)
		throw AppException("Scop not running, call .loop()");
	
	static bool firstRun = true;
	if (firstRun) {
		this->_currCursorX = posX;
		this->_currCursorY = posY;
		firstRun = false;
	}
		
	float yaw = (posX - this->_currCursorX) * SCOP_CAMERA_SENITIVITY;
	float pitch = (this->_currCursorY - posY) * SCOP_CAMERA_SENITIVITY;  // reversed since y-coordinates range from bottom to top

	if (pitch > 89.0f)
		pitch =  89.0f;
	else if(pitch < -89.0f)
		pitch = -89.0f;

	this->_camera->rotate(pitch, yaw, 0.0f);
	this->_currCursorX = posX;
	this->_currCursorY = posY;
}
