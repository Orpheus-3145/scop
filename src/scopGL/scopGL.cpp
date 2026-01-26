#include "scopGL/scopGL.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void pressEscCb( GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods ) {
	(void) scancode; (void) mods;
	if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void resizeCb( GLFWwindow* window, int32_t width, int32_t height ) {
	(void) window;
	glViewport(0, 0, width, height);
}

ScopGL::ScopGL( void ) {
	this->_window = nullptr;
	this->_shaderProgram = 0U;
	this->_VBO = 0U;
	this->_VAO = 0U;
	this->_EBO = 0U;
	this->_texture = 0U;
}

ScopGL::~ScopGL( void ) {
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

	data.triangulation();
	data.mapTextures();
	data.fillBuffers();
	this->_VBOdata = data.getVBO();
	if (data.hasFaces())
		this->_EBOdata = data.getEBO();
	std::cout << "parsed file " << fileName << std::endl;
}

void ScopGL::createWindow( int32_t width, int32_t height ) {
	if (this->_window)
		throw GlfwException("already initialized");
	else if (width < 0 or height < 0)
		throw GlfwException("negative value for width or height");
	else if (!glfwInit()) {
		const char* description;
		glfwGetError(&description);
		throw GlfwException("initialization of glfw failed: " + std::string(description));
	}
	std::cout << "glfw initialized, version: " << GLFW_CONTEXT_VERSION_MAJOR << "." << GLFW_CONTEXT_VERSION_MINOR << "." << GLFW_VERSION_REVISION << std::endl;

	this->_currentWidth = width;
	this->_currentHeight = height;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	this->_window = glfwCreateWindow(this->_currentWidth, this->_currentHeight, "SCOP", nullptr, nullptr);
	if (!this->_window) {
		const char* description;
		glfwGetError(&description);
		throw GlfwException("creation of window failed: " + std::string(description));
	}
	std::cout << "created window: " << this->_currentWidth << "x" << this->_currentHeight << std::endl;
	
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int32_t posX = (mode->width - this->_currentWidth) / 2;
	int32_t posY = (mode->height - this->_currentHeight) / 2;
	glfwSetWindowPos(this->_window, posX, posY);
	
	glfwMakeContextCurrent(this->_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw GlfwException("initialization of GLAD failed");
	std::cout << "using GLAD" << std::endl;
	glfwSetWindowUserPointer(_window, this);
	// callbacks
	glfwSetKeyCallback(this->_window, pressEscCb);
	glfwSetFramebufferSizeCallback(this->_window, []( GLFWwindow* window, int32_t w, int32_t h) {
		ScopGL* self = static_cast<ScopGL*>(glfwGetWindowUserPointer(window));
		resizeCb(window, w, h);
		self->_currentWidth = w;
		self->_currentHeight = h;
	});
	std::cout << "setup callbacks for resize event and ESC key" << std::endl;
}

void ScopGL::initGL( std::string const& vertexShaderSource, std::string const& textureShaderSource, std::string const& textureFile ) {
	if (!this->_window)
		throw AppException("GLFW not started, call .createWindow()");
	if (this->_shaderProgram)
		throw AppException("setup openGL already done");
	
	glViewport(0, 0, this->_currentWidth, this->_currentWidth);

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
}

void ScopGL::sendBuffersToGPU( void ) {
	if (!this->_window)
		throw AppException("GLFW not started, call .createWindow()");
	else if (!this->_VBOdata)
		throw AppException("fata not parsed, call .parseFile()");
	else if (this->_VBO != 0U)
		throw AppException("buffers already sent to GPU");

	glGenBuffers(1, &this->_VBO);
	if (this->_EBOdata)
		glGenBuffers(1, &this->_EBO);
	glGenVertexArrays(1, &this->_VAO);
	glBindVertexArray(this->_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	
	// load vertex data
	glBufferData(GL_ARRAY_BUFFER, this->_VBOdata->size * this->_VBOdata->stride, this->_VBOdata->getData(), GL_STATIC_DRAW);
	std::cout << "VBO uploaded to GPU" << std::endl;
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
		std::cout << "EBO uploaded to GPU" << std::endl;
	}
	glBindVertexArray(0);
}

void ScopGL::start( void ) {
	if (!this->_VBOdata)
		throw AppException("data not parsed, call .parseFile()");
	if (!this->_window)
		throw AppException("GLFW not started, call .createWindow()");
	else if (!this->_VBO)
		throw AppException("buffers not sent to GPU, call .sendBuffersToGPU()");
	
	std::cout << "opening window" << std::endl;
	Matrix4 model = createIdMat();
	Matrix4 view = createIdMat();
	Matrix4 projection = createIdMat();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!glfwWindowShouldClose(this->_window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, this->_texture);
		glUseProgram(this->_shaderProgram);
		glBindVertexArray(this->_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);

		GLint modelLoc = glGetUniformLocation(this->_shaderProgram, "model");
		GLint viewLoc = glGetUniformLocation(this->_shaderProgram, "view");
		GLint projectionLoc = glGetUniformLocation(this->_shaderProgram, "projection");

		model = rotationMat(toRadiants(80 * glfwGetTime()), {.0f, -1.0f / sqrtf(2), -1.0f / sqrtf(2)});		// transMat({.0f, .0f, -1.f}) * 
		view = transMat({.0f, .0f, -5.f});
		projection = projectionMatFinite(45.0f, (float)SCOP_WINDOW_WIDTH / (float)SCOP_WINDOW_HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
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
		throw AppException("failed to link shaders: " + std::string(infoLog));
	}
}

uint32_t ScopGL::_loadShader( GLenum type, std::string const& fileName ) {
	std::ifstream readFile;

	readFile.open(fileName, std::ifstream::in);
	if (!readFile)
		throw AppException("failed to open shader file: " + fileName);

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
		throw AppException("failed to compile shader: " + fileName + ", trace: " + infoLog);
	}
	return shaderRef;
}

void ScopGL::_loadTexture( std::string const& texturePath ) {
	glGenTextures(1, &this->_texture);
	glBindTexture(GL_TEXTURE_2D, this->_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int32_t width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
		throw AppException("failed to load texture in: " + texturePath);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}
