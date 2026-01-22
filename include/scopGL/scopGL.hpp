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
		void createWindow( int32_t, int32_t );
		void initGL( std::string const&, std::string const&, std::string const& );
		void sendBuffersToGPU( void );
		void start( void );

	private:
		GLFWwindow*				_window;
		GLuint					_texture;
		GLuint					_shaderProgram;
		GLuint					_VBO;
		GLuint					_EBO;
		GLuint					_VAO;
		std::shared_ptr<VBO>	_VBOdata;
		std::shared_ptr<EBO>	_EBOdata;
		GLuint					_currentWidth;
		GLuint					_currentHeight;
		
		void 		_createShader( GLenum type, std::string const&);
		uint32_t	_loadShader( GLenum, std::string const& );
		void 		_loadTexture( std::string const& );
};
