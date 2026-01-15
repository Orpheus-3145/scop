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
#include "scopGL/scopMath.hpp"


class ScopGL {
	public:
		ScopGL( void );
		~ScopGL( void ) noexcept;

		void parseFile( std::string const& );
		void createWindow( size_t, size_t );
		void createShaders( std::multimap<int, std::string> const& );
		void loadData( void );
		void loadTexture( std::string const& );
		void start( void );

	private:
		std::shared_ptr<ParsedData>	_parsed;
		std::shared_ptr<VBO>		_VBOdata;
		std::shared_ptr<EBO>		_EBOdata;
		GLFWwindow*					_currentWindow;
		unsigned int				_shaderProgram;
		unsigned int				_VBO;
		unsigned int				_EBO;
		unsigned int				_VAO;
		unsigned int				_texture;

		unsigned int _loadShader( int, std::string const& );
};
