#pragma once
#include <GLFW/glfw3.h>

class ScopWindow {
	private:
		GLFWwindow* currentWindow;

	public:
		ScopWindow( void );
		~ScopWindow( void ) noexcept;

		void open(unsigned int width, unsigned int height);
};
