#include "window/scopWindow.hpp"


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

ScopWindow::ScopWindow( void ) {
	this->_currentWindow = nullptr;
	if (!glfwInit()) {
		const char* description;
		glfwGetError(&description);
		throw WindowException("initialization of glfw failed: " + std::string(description));
	}

	std::cout << "glfw initialized, using version: " << GLFW_CONTEXT_VERSION_MAJOR << "." << GLFW_CONTEXT_VERSION_MINOR << "." << GLFW_VERSION_REVISION << std::endl;
}

ScopWindow::~ScopWindow( void ) noexcept {
	if (this->_currentWindow)
		glfwDestroyWindow(this->_currentWindow);
	glfwTerminate();
}

void ScopWindow::init(size_t width, size_t height) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	this->_currentWindow = glfwCreateWindow(width, height, "SCOP", nullptr, nullptr);
	if (!this->_currentWindow) {
		const char* description;
		glfwGetError(&description);
		throw WindowException("creation of the window failed: " + std::string(description));
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int posX = (mode->width - width) / 2;
	int posY = (mode->height - height) / 2;
	glfwSetWindowPos(this->_currentWindow, posX, posY);

	glfwMakeContextCurrent(this->_currentWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw WindowException("initialization of GLAD failed");
	glViewport(0, 0, width, height);
	// callbacks
	glfwSetKeyCallback(this->_currentWindow, pressEscCb);
	glfwSetFramebufferSizeCallback(this->_currentWindow, resizeCb);
}

bool ScopWindow::initiliazed( void ) const noexcept {
	return this->_currentWindow != nullptr;
}

void ScopWindow::startLoop( void ) {
	// render loop
	while (!glfwWindowShouldClose(this->_currentWindow)) {
		glClearColor(0.8f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(this->_currentWindow);
	}

}
