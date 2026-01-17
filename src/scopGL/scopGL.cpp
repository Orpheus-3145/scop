#include "scopGL/scopGL.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void pressEscCb(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	(void) scancode; (void) mods;
	if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void resizeCb(GLFWwindow* window, int32_t width, int32_t height) {
	(void) window;
	glViewport(0, 0, width, height);
}

ScopGL::ScopGL( void ) {
	this->_currentWindow = nullptr;
	this->_shaderProgram = 0;
	this->_VBO = 0;
	this->_VAO = 0;
	this->_EBO = 0;
	this->_texture = 0;
	if (!glfwInit()) {
		const char* description;
		glfwGetError(&description);
		throw GlfwException("initialization of glfw failed: " + std::string(description));
	}
	std::cout << "glfw initialized, version: " << GLFW_CONTEXT_VERSION_MAJOR << "." << GLFW_CONTEXT_VERSION_MINOR << "." << GLFW_VERSION_REVISION << std::endl;
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
	if (this->_currentWindow)
		glfwDestroyWindow(this->_currentWindow);
	glfwTerminate();
}

void ScopGL::parseFile( std::string const& fileName ) {
	FileParser parser;
	this->_parsed = parser.parse(fileName);
	this->_parsed->createBuffers();
	std::cout << "parsed file " << fileName << std::endl;
	// std::cout << *this->_parsed->getVBO();
	// std::cout << *this->_parsed->getEBO();
}

void ScopGL::createWindow( int32_t width, int32_t height ) {
	if (this->_currentWindow)
		throw GlfwException("already initialized");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	this->_currentWindow = glfwCreateWindow(width, height, "SCOP", nullptr, nullptr);
	if (!this->_currentWindow) {
		const char* description;
		glfwGetError(&description);
		throw GlfwException("creation of window failed: " + std::string(description));
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int32_t posX = (mode->width - width) / 2;
	int32_t posY = (mode->height - height) / 2;
	glfwSetWindowPos(this->_currentWindow, posX, posY);

	glfwMakeContextCurrent(this->_currentWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw GlfwException("initialization of GLAD failed");
	// callbacks
	glfwSetKeyCallback(this->_currentWindow, pressEscCb);
	glfwSetFramebufferSizeCallback(this->_currentWindow, resizeCb);
	glViewport(0, 0, width, height);
	std::cout << "created window: " << width << "x" << height << std::endl;
}

void ScopGL::initGL( void ) {
	if (!this->_parsed)
		throw AppException("Data not parsed, call .parseFile()");

	this->_loadTexture("resources/textures/capybara.jpg");

	this->_createShaders({
		{GL_VERTEX_SHADER, "resources/shaders/vertexShaderTest.glsl"},
		{GL_FRAGMENT_SHADER, "resources/shaders/fragmentShaderTest.glsl"}
	});

	glGenBuffers(1, &this->_VBO);
	glGenBuffers(1, &this->_EBO);
	glGenVertexArrays(1, &this->_VAO);

	std::shared_ptr<VBO> const& VBOdata = this->_parsed->getVBO();
	std::shared_ptr<EBO> const& EBOdata = this->_parsed->getEBO();

	glBindVertexArray(this->_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
	// load vertex data
	glBufferData(GL_ARRAY_BUFFER, VBOdata->size * VBOdata->stride * sizeof(float), VBOdata->getData(), GL_STATIC_DRAW);
	// load indexes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBOdata->size * EBOdata->stride * sizeof(uint32_t), EBOdata->getData(), GL_STATIC_DRAW);
	// vertex metadata in VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VBOdata->stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color metadata in VAO
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VBOdata->stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture metadata in VAO
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VBOdata->stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normals metadata in VAO
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, VBOdata->stride * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ScopGL::start( void ) {
	if (!this->_parsed)
		throw AppException("Data not parsed, call .parseFile()");
	if (!this->_currentWindow)
		throw AppException("GLFW not started, call .createWindow()");

	Matrix4 model = createIdMat();
	Matrix4 view = createIdMat();
	Matrix4 projection = createIdMat();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!glfwWindowShouldClose(this->_currentWindow)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, this->_texture);
		glUseProgram(this->_shaderProgram);
		glBindVertexArray(this->_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);

		uint32_t viewLoc = glGetUniformLocation(this->_shaderProgram, "view");
		uint32_t projectionLoc = glGetUniformLocation(this->_shaderProgram, "projection");
		uint32_t modelLoc = glGetUniformLocation(this->_shaderProgram, "model");

		// model = transMat({.0f, .0f, -5.f});
		model = rotationMat(toRadiants(80.0f * glfwGetTime()), {1.0f / sqrtf(2.f), 1.0f / sqrtf(2.f), .0f});
		view = transMat({.0f, .0f, -5.f});
		projection = projectionMatFinite(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
		std::shared_ptr<EBO> const& EBOdata = this->_parsed->getEBO();

		// glDrawArrays(GL_TRIANGLES, 0, this->_VBOdata->size);
		glDrawElements(GL_TRIANGLES, EBOdata->size * EBOdata->stride, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(this->_currentWindow);
		glfwPollEvents();
	}
}

uint32_t ScopGL::_loadShader(uint32_t type, std::string const& fileName) {
	std::ifstream readFile;

	readFile.open(fileName, std::ifstream::in);
	if (!readFile)
		throw AppException("Failed to open shader file: " + fileName);

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
		throw AppException("Failed to compile shader: " + fileName + ", trace: " + infoLog);
	}
	return shaderRef;
}

void ScopGL::_createShaders( std::multimap<uint32_t, std::string> const& inputShaders) {
	this->_shaderProgram = glCreateProgram();

	for (const auto& [shType, shPath] : inputShaders) {
		uint32_t shader = this->_loadShader(shType, shPath);
		glAttachShader(this->_shaderProgram, shader);
		glDeleteShader(shader);
	}
	glLinkProgram(this->_shaderProgram);

	// check linking status
	int32_t  success;
	char infoLog[512];
	glGetProgramiv(this->_shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(this->_shaderProgram, 512, NULL, infoLog);
		throw AppException("Failed to link shaders: " + std::string(infoLog));
	}
}

void ScopGL::_loadTexture( std::string const& texturePath ) {
	glGenTextures(1, &this->_texture);
	glBindTexture(GL_TEXTURE_2D, this->_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int32_t width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
		throw AppException("Failed to load texture in: " + texturePath);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

