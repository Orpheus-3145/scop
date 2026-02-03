#include "scop.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



GraphicGL::GraphicGL( GLuint shader, std::string const& uniformName ) {
	this->_shader = shader;
	this->_transformation = idMat();

	this->_shaderLocation = glGetUniformLocation(this->_shader, uniformName.data());
	if (this->_shaderLocation == -1)
		throw OpenGlException("Uniform variable '" + uniformName + "' not found in shader");
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


void ModelGL::rotate( float tetha, VectF3 const& rotAxis ) noexcept {
	this->_transformation *= rotationMat(tetha, rotAxis);
}

void ModelGL::translate( VectF3 const& trans ) noexcept {
	this->_transformation *= transMat(trans);
}

void ModelGL::scale( VectF3 const& scale ) noexcept {
	this->_transformation *= scaleMat(scale);
}

void ModelGL::updateShader( void ) {
	GraphicGL::updateShader();
	this->reset();
}


void CameraGL::moveForward( float delta ) noexcept {
	VectF3 cameraTarget = this->_position + this->_forward;
	VectF3 cameraForward = normalize(this->_position - cameraTarget);
	this->_position -= cameraForward * delta;
	this->updateShader();
}

void CameraGL::moveBackward( float delta ) noexcept {
	VectF3 cameraTarget = this->_position + this->_forward;
	VectF3 cameraForward = normalize(this->_position - cameraTarget);
	this->_position += cameraForward * delta;
	this->updateShader();
}

void CameraGL::moveRight( float delta ) noexcept {
	VectF3 cameraTarget = this->_position + this->_forward;
	VectF3 cameraForward = normalize(this->_position - cameraTarget);
	VectF3 cameraLeft = normalize(this->__up ^ cameraForward );
	this->_position += cameraLeft * delta;
	this->updateShader();
}

void CameraGL::moveLeft( float delta ) noexcept {
	VectF3 cameraTarget = this->_position + this->_forward;
	VectF3 cameraForward = normalize(this->_position - cameraTarget);
	VectF3 cameraLeft = normalize(this->__up ^ cameraForward );
	this->_position -= cameraLeft * delta;
	this->updateShader();
}

void CameraGL::rotate( float pitch, float yaw, float roll ) noexcept {
	pitch = toRadiants(pitch / 2.0f);	// vertical rotation: x is the axis
	yaw = toRadiants(yaw / 2.0f);		// horizontal rotation: y is the axis

	Quatern qPitch{cosf(pitch), sinf(pitch), 0.0f, 0.0f};
	Quatern qYaw{cosf(-yaw), 0.0f, sinf(-yaw), 0.0f};
	Quatern qRoll{cosf(roll), 0.0f, 0.0f, sinf(roll)};

	// NB check this
	// VectF3 cameraTarget = this->_position + this->_forward;
	// VectF3 cameraForward = normalize(this->_position - cameraTarget);
	// VectF3 cameraLeft = normalize(this->__up ^ cameraForward );
	// Quatern qYaw{-yaw, cameraLeft};
	// Quatern qPitch(pitch, this->__up);

	Quatern q = qPitch * qYaw * qRoll;		// rotation order: roll -> yaw -> pitch
	Quatern p{0.0f, this->_forward.x, this->_forward.y, this->_forward.z};
	Quatern p_ = (q * p) * q.conjugate();
	this->_forward = p_.vector();
	this->updateShader();
}

void CameraGL::updateShader( void ) {
	VectF3 cameraTarget = this->_position + this->_forward;
	VectF3 cameraForward = normalize(this->_position - cameraTarget);
	VectF3 cameraLeft = normalize(this->__up ^ cameraForward );
	VectF3 cameraUp = cameraForward ^ cameraLeft;

	Matrix4 lookAt{std::array<float,16>{
		cameraLeft.x,     cameraLeft.y,     cameraLeft.z,     (this->_position * cameraLeft) * -1,
		cameraUp.x,       cameraUp.y,       cameraUp.z,       (this->_position * cameraUp) * -1,
		cameraForward.x,  cameraForward.y,  cameraForward.z,  (this->_position * cameraForward) * -1,
		0.0f,             0.0f,             0.0f,             1.0f
	}};
	this->_transformation = lookAt;
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


ScopGL::ScopGL( void ) noexcept {
	this->_window = nullptr;
	this->_texture = 0U;
	this->_shaderProgram = 0U;
	this->_VBO = 0U;
	this->_EBO = 0U;
	this->_VAO = 0U;
	this->_VBOdata = std::shared_ptr<VBO>();
	this->_EBOdata = std::shared_ptr<EBO>();
	this->_applyTextures = false;
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
	glfwSetWindowUserPointer(_window, this);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int32_t posX = (mode->width - width) / 2;
	int32_t posY = (mode->height - height) / 2;
	glfwSetWindowPos(this->_window, posX, posY);

	glfwMakeContextCurrent(this->_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw GlfwException("Initialization of GLAD failed");
	std::cout << "using GLAD" << std::endl;
}

void ScopGL::initGL( std::string const& vertexShaderSource, std::string const& textureShaderSource, std::string const& textureFile ) {
	if (!this->_window)
		throw AppException("GLFW not started, call .createWindow()");
	else if (this->_shaderProgram)
		throw AppException("Setup openGL already done");

	int32_t width, height;
	glfwGetWindowSize(this->_window, &width, &height);
	glViewport(0, 0, width, height);

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
	this->_camera = std::make_unique<CameraGL>(this->_shaderProgram, VectF3{0.0f, 0.0f, 8.0f});
	this->_projection = std::make_unique<ProjectionGL>(this->_shaderProgram, width, height);
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

	int32_t width, height;
	glfwGetWindowSize(this->_window, &width, &height);
	glfwSetCursorPos(this->_window, width / 2, height / 2);
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
	if(!success)
	{
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
	glfwSetFramebufferSizeCallback(this->_window, []( GLFWwindow* window, int32_t w, int32_t h) {
		ScopGL* self = static_cast<ScopGL*>(glfwGetWindowUserPointer(window));
		if (self)
			self->_resetWindowSize(w, h);
	});

	glfwSetKeyCallback(this->_window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
		(void) scancode; (void) mods;
		ScopGL* self = static_cast<ScopGL*>(glfwGetWindowUserPointer(window));
		if (!self)
			return;

		if (key == GLFW_KEY_T and action == GLFW_PRESS)
			self->_toggleTextures();
		else if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS)
			self->_closeWindow();
	});

	glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(this->_window, [](GLFWwindow* window, double posX, double posY) {
		ScopGL* self = static_cast<ScopGL*>(glfwGetWindowUserPointer(window));
		if (!self)
			return;
		self->_calcolateAngle(posX, posY);
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
	static float lastFrame = 0.0f;
	float currentFrame = glfwGetTime();
	float deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	constexpr float rotationDelta = 0.3f;

	if (glfwGetKey(this->_window, GLFW_KEY_W) == GLFW_PRESS)
		this->_camera->moveForward(deltaTime * SCOP_CAMERA_SPEED);
	else if (glfwGetKey(this->_window, GLFW_KEY_S) == GLFW_PRESS)
		this->_camera->moveBackward(deltaTime * SCOP_CAMERA_SPEED);
	else if (glfwGetKey(this->_window, GLFW_KEY_A) == GLFW_PRESS)
		this->_camera->moveLeft(deltaTime * SCOP_CAMERA_SPEED);
	else if (glfwGetKey(this->_window, GLFW_KEY_D) == GLFW_PRESS)
		this->_camera->moveRight(deltaTime * SCOP_CAMERA_SPEED);
	else if (glfwGetKey(this->_window, GLFW_KEY_UP) == GLFW_PRESS)
		this->_camera->rotate(rotationDelta * SCOP_CAMERA_SPEED, 0.0f, 0.0f);
	else if (glfwGetKey(this->_window, GLFW_KEY_DOWN) == GLFW_PRESS)
		this->_camera->rotate(-rotationDelta * SCOP_CAMERA_SPEED, 0.0f, 0.0f);
	else if (glfwGetKey(this->_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		this->_camera->rotate(0.0f, -rotationDelta * SCOP_CAMERA_SPEED, 0.0f);
	else if (glfwGetKey(this->_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		this->_camera->rotate(0.0f, rotationDelta * SCOP_CAMERA_SPEED, 0.0f);
}

void ScopGL::_resetWindowSize( uint32_t width, uint32_t height ) {
	if (!this->_shaderProgram)
		throw AppException("OpenGL not started, call .initGL()");

	this->_projection->setAspect(width, height);
	glViewport(0, 0, width, height);
}

void ScopGL::_closeWindow( void ) {
	if (!this->_window)
		throw AppException("Window not created, call .createWindow()");

	glfwSetWindowShouldClose(this->_window, GLFW_TRUE);
}

void ScopGL::_toggleTextures( void ) {
	if (!this->_shaderProgram)
		throw AppException("OpenGL not started, call .initGL()");

	this->_applyTextures = !this->_applyTextures;
	GLboolean applyTexture = glGetUniformLocation(this->_shaderProgram, "applyTexture");
	glUniform1i(applyTexture, this->_applyTextures);
}

void ScopGL::_calcolateAngle( float posX, float posY ) {
	int32_t width, height;
	glfwGetWindowSize(this->_window, &width, &height);

	static bool firstMouse = true;
	static float lastX = static_cast<float>(width) / 2.0f;
	static float lastY = static_cast<float>(height) / 2.0f;

	if (firstMouse) {
		lastX = posX;
		lastY = posY;
		firstMouse = false;
	}

	float offsetX = (posX - lastX) * SCOP_CAMERA_SENITIVITY;
	float offsetY = (lastY - posY) * SCOP_CAMERA_SENITIVITY;  // reversed since y-coordinates range from bottom to top
	lastX = posX;
	lastY = posY;

	float yaw = offsetX;
	float pitch = offsetY;
	if(pitch > 89.0f)
		pitch =  89.0f;
	else if(pitch < -89.0f)
		pitch = -89.0f;

	this->_camera->rotate(pitch, yaw, 0.0f);
}
