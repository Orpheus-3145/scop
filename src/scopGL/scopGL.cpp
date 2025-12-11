#include "scopGL/scopGL.hpp"


void pressEscCb(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void) scancode; (void) mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void resizeCb(GLFWwindow* window, int width, int height)
{
	(void) window;
	glViewport(0, 0, width, height);
}

ScopGL::ScopGL( void ) {
	this->_currentWindow = nullptr;
	this->_shaderProgram = 0;
	this->_VBO = 0;
	this->_VAO = 0;
	if (!glfwInit()) {
		const char* description;
		glfwGetError(&description);
		throw GlfwException("initialization of glfw failed: " + std::string(description));
	}
	std::cout << "glfw initialized, version: " << GLFW_CONTEXT_VERSION_MAJOR << "." << GLFW_CONTEXT_VERSION_MINOR << "." << GLFW_VERSION_REVISION << std::endl;
}

ScopGL::~ScopGL( void ) noexcept {

	for (unsigned int vao : this->_VAOs)
		glDeleteVertexArrays(1, &vao);
	for (unsigned int vbo : this->_VBOs)
		glDeleteBuffers(1, &vbo);

	if (this->_shaderProgram)
		glDeleteProgram(this->_shaderProgram);
	if (this->_currentWindow)
		glfwDestroyWindow(this->_currentWindow);
	glfwTerminate();
}

void ScopGL::parseFile( std::string const& fileName ) {
	FileParser parser;
	this->_dataParsed = std::move(parser.parse(fileName));
	std::cout << "parsed file " << fileName << std::endl;
}

void ScopGL::createWindow( size_t width, size_t height ) {
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
	int posX = (mode->width - width) / 2;
	int posY = (mode->height - height) / 2;
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

void ScopGL::createShaders( std::multimap<int, std::string> const& inputShaders) {
	this->_shaderProgram = glCreateProgram();

	for (const auto& [shType, shPath] : inputShaders) {
		unsigned int shader = this->_loadShader(shType, shPath);
		glAttachShader(this->_shaderProgram, shader);
		glDeleteShader(shader);
	}
	glLinkProgram(this->_shaderProgram);

	// check linking status
	int  success;
	char infoLog[512];
	glGetProgramiv(this->_shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(this->_shaderProgram, 512, NULL, infoLog);
		throw AppException("Failed to link shaders: " + std::string(infoLog));
	}
}

void ScopGL::loadData( void ) {
	if (!this->_dataParsed)
		throw AppException("Data not parsed, call .parseFile() first");

	BufferData data = this->_dataParsed->getVertexData();
	if (data.getSize() > 0) {
		this->createShaders({
			{GL_VERTEX_SHADER, "resources/shaders/vertexShaderTest.glsl"},
			{GL_FRAGMENT_SHADER, "resources/shaders/fragmentShaderTest.glsl"}
		});
		glGenVertexArrays(1, &this->_VAO);
		glGenBuffers(1, &this->_VBO);
		glBindVertexArray(this->_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
		glBufferData(GL_ARRAY_BUFFER, data.getSize() * data.getDimension() * sizeof(double), data.getData(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, data.getDimension(), GL_DOUBLE, GL_FALSE, data.getDimension() * sizeof(double), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
		glBindVertexArray(0);
		std::cout << "data size: " << data.getSize() << " dimension: " << data.getDimension() << std::endl;
	}

	// std::unique_ptr<double[]> textureData = this->_dataParsed->getTextureData();
	// std::unique_ptr<double[]> vertexNormData = this->_dataParsed->getVertexNormData();
	// std::unique_ptr<double[]> paramSpaceVerticexData = this->_dataParsed->getParamSpaceVertexData();
}

void ScopGL::start( void ) {
	if (!this->_dataParsed)
		throw AppException("Data not parsed, call .parseFile() first");
	if (!this->_currentWindow)
		throw AppException("GLFW not started, call .createWindow() first");

	while (!glfwWindowShouldClose(this->_currentWindow)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// for every object
		// glBindVertexArray(vaoX);
		// glUseProgram(shaderX);
		// glBindTexture(GL_TEXTURE_2D, texX);
		// glDrawArrays(GL_TRIANGLES, 0, X);

		glUseProgram(this->_shaderProgram);
		glBindVertexArray(this->_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 353);

		glfwSwapBuffers(this->_currentWindow);
		glfwPollEvents();
	}
    // glDeleteVertexArrays(1, &this->_VAO);
    // glDeleteBuffers(1, &this->_VBO);
}

void ScopGL::doTwoTrianglesTest( void ) {
	float vertices[] = {
		-1.f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.75f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		1.f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	this->createShaders({
		{GL_VERTEX_SHADER, "resources/shaders/vertexShaderTest.glsl"},
		{GL_FRAGMENT_SHADER, "resources/shaders/fragmentShaderTest.glsl"}
	});
	glGenVertexArrays(1, &this->_VAO);  
	glGenBuffers(1, &this->_VBO);  
	glBindVertexArray(this->_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0); 
}

unsigned int ScopGL::_loadShader(int type, std::string const& fileName) {
	std::ifstream readFile;

	readFile.open(fileName, std::ifstream::in);
	if (!readFile)
		throw AppException("Failed to open shader file: " + fileName);

	std::stringstream buffer;
	buffer << readFile.rdbuf();
	readFile.close();

	std::string fileContentStr = buffer.str();
	const char* fileContent = fileContentStr.c_str();
	unsigned int shaderRef = glCreateShader(type);
	glShaderSource(shaderRef, 1, &fileContent, NULL);
	glCompileShader(shaderRef);

	// check compilation status
	int  success;
	char infoLog[512];
	glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shaderRef, 512, NULL, infoLog);
		throw AppException("Failed to compile shader: " + fileName + ", trace: " + infoLog);
	}
	return shaderRef;
}
