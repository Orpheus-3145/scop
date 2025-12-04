#pragma once
#include <vector>
#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "exceptions.hpp"


class ScopWindow {
	public:
		ScopWindow( void ) noexcept;
		~ScopWindow( void ) noexcept;

		void init( size_t, size_t );
		bool initiliazed( void ) const noexcept;
		void startLoop( void );

	private:
		GLFWwindow*			_currentWindow;
		std::vector<int>	_shaders;
};
