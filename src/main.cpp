#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "define.hpp"


void pressEscCb(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void)scancode; (void)mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char** argv) {
	(void) argc; (void) argv;

	if (!glfwInit()) {
		const char* description;
		glfwGetError(&description);
		std::cout << "initialization of glfw failed: " << description << std::endl;	
		return (EXIT_FAILURE);
	}
	std::cout << "glfw initialized, using version: " << GLFW_CONTEXT_VERSION_MAJOR << "." << GLFW_CONTEXT_VERSION_MINOR << std::endl;

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
	glfwSetKeyCallback(newWindow, pressEscCb);

	while (!glfwWindowShouldClose(newWindow)) {
		// glfwSwapBuffers(newWindow);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(newWindow);
	glfwTerminate();

	return (EXIT_SUCCESS);
}