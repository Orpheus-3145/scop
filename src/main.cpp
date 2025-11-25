#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "define.hpp"


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


int main(int argc, char** argv) {
	(void) argc; (void) argv;

	if (!glfwInit()) {
		const char* description;
		glfwGetError(&description);
		std::cout << "initialization of glfw failed: " << description << std::endl;	
		return (EXIT_FAILURE);
	}
	std::cout << "glfw initialized, using version: " << GLFW_CONTEXT_VERSION_MAJOR << "." << GLFW_CONTEXT_VERSION_MINOR << "." << GLFW_VERSION_REVISION << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* newWindow = glfwCreateWindow(1920, 1080, "test win", nullptr, nullptr);
	if (!newWindow) {
		const char* description;
		glfwGetError(&description);
		std::cout << "creation of window failed: " << description << std::endl;	
		glfwTerminate();
		return (EXIT_FAILURE);
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int posX = (mode->width - WINDOW_WIDTH) / 2;
	int posY = (mode->height - WINDOW_HEIGHT) / 2;
	glfwSetWindowPos(newWindow, posX, posY);

	glfwMakeContextCurrent(newWindow);
	gladLoadGL();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// callbacks
	glfwSetKeyCallback(newWindow, pressEscCb);
	glfwSetFramebufferSizeCallback(newWindow, resizeCb);

	// render loop
	while (!glfwWindowShouldClose(newWindow)) {
		glClearColor(0.8f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(newWindow);
	}
	
	glfwDestroyWindow(newWindow);
	glfwTerminate();

	return (EXIT_SUCCESS);
}