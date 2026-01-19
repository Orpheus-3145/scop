#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "define.hpp"
#include "exceptions.hpp"
#include "parser.hpp"
#include "scopGL/scopMath.hpp"


void pressEscCb(GLFWwindow*, int32_t, int32_t, int32_t, int32_t);
void resizeCb(GLFWwindow*, int32_t, int32_t );

class ScopGL {
	public:
		ScopGL( void );
		~ScopGL( void );

		void parseFile( std::string const& );
		void initGL( void );
		void start( void );

	private:
		GLFWwindow*					_currentWindow;
		uint32_t					_texture;
		uint32_t					_shaderProgram;
		uint32_t					_VBO;
		uint32_t					_EBO;
		uint32_t					_VAO;
		std::shared_ptr<VBO>		_VBOdata;
		std::shared_ptr<EBO>		_EBOdata;
		
		void 						_createWindow( int32_t, int32_t );
		void 						_createShaders( std::multimap<uint32_t, std::string> const& );
		void 						_loadTexture( std::string const& );
		uint32_t					_loadShader( uint32_t, std::string const& );
};
