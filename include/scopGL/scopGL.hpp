#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "define.hpp"
#include "exceptions.hpp"
#include "parser/parser.hpp"


class ScopGL {
	public:
		ScopGL( void );
		~ScopGL( void ) noexcept;

		void parseFile( std::string const& );
		void createWindow( size_t, size_t );
		void createShaders( std::multimap<int, std::string> const& );
		void loadData( void );
		void start( void );

		void doTwoTrianglesTest( void );

	private:
		std::unique_ptr<ParsedData>	_parsed;
		std::unique_ptr<RawData>	_raw;
		GLFWwindow*					_currentWindow;
		unsigned int				_shaderProgram;
		unsigned int				_VBO;
		unsigned int				_EBO;
		unsigned int				_VAO;

		unsigned int _loadShader( int, std::string const& );
};
