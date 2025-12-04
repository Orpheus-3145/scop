#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "define.hpp"
#include "exceptions.hpp"
#include "window/scopWindow.hpp"
#include "parser/parser.hpp"


class ScopGL {
	public:
		ScopGL( void ) noexcept {};
		~ScopGL( void ) noexcept {};

		void parseFile( std::string const& );
		void createWindow( void );
		void start( void );
		void loadShader( int, std::string const& );

	private:
		FileParser  _parser;
		ScopWindow  _window;
};
