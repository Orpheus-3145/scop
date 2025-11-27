#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "window/scopWindow.hpp"
#include "exceptions.hpp"


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
	this->currentWindow = nullptr;
	if (!glfwInit()) {
		const char* description;
		glfwGetError(&description);
		throw WindowException("initialization of glfw failed: " + std::string(description));
	}
	std::cout << "glfw initialized, using version: " << GLFW_CONTEXT_VERSION_MAJOR << "." << GLFW_CONTEXT_VERSION_MINOR << "." << GLFW_VERSION_REVISION << std::endl;

}

ScopWindow::~ScopWindow( void ) noexcept {
	if (this->currentWindow)
		glfwDestroyWindow(this->currentWindow);
	glfwTerminate();
}

void ScopWindow::open(unsigned int width, unsigned int height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	this->currentWindow = glfwCreateWindow(width, height, "SCOP", nullptr, nullptr);
	if (!this->currentWindow) {
		const char* description;
		glfwGetError(&description);
		throw WindowException("creation of the window failed: " + std::string(description));
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int posX = (mode->width - width) / 2;
	int posY = (mode->height - height) / 2;
	glfwSetWindowPos(this->currentWindow, posX, posY);

	glfwMakeContextCurrent(this->currentWindow);
	gladLoadGL();
	glViewport(0, 0, width, height);
	// callbacks
	glfwSetKeyCallback(this->currentWindow, pressEscCb);
	glfwSetFramebufferSizeCallback(this->currentWindow, resizeCb);

	// render loop
	while (!glfwWindowShouldClose(this->currentWindow)) {
		glClearColor(0.8f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(this->currentWindow);
	}
}