#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// #include <string>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>


int main(int argc, char** argv) {
	(void) argc; (void) argv;

	if (!glfwInit()) {
		// to set a callback in case of error: glfwSetErrorCallback(error_callback) where: void error_callback(int error, const char* description) {}
		const char* description;
		glfwGetError(&description);
		std::cout << "initialization of glfw failed: " << description << std::endl;	
		return (EXIT_FAILURE);
	}
	std::cout << "glfw initialized, using version: " << GLFW_CONTEXT_VERSION_MAJOR << "." << GLFW_CONTEXT_VERSION_MINOR << std::endl;

	GLFWwindow* newWindow = glfwCreateWindow(640, 480, "test win", nullptr, nullptr);

	if (!newWindow) {
		const char* description;
		glfwGetError(&description);
		std::cout << "creation of window failed: " << description << std::endl;	
		glfwTerminate();
		return (EXIT_FAILURE);
	}

	while (!glfwWindowShouldClose(newWindow)) {}
	
	glfwDestroyWindow(newWindow);

	glfwTerminate();
	return (EXIT_SUCCESS);
}