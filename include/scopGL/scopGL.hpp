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

class CameraGL {
	public:
		CameraGL( VectF3 const& pos, VectF3 const& front ) noexcept :
			_cameraPos(pos), 
		  	_cameraFront(front), 
		  	_cameraUp(VectF3{0.0f, 1.0f, 0.0f}) {
				this->_update();
			};

		~CameraGL( void ) = default;

		void			move_forward( void ) noexcept;
		void			move_backward( void ) noexcept;
		void			rotate_right( void ) noexcept;
		void			rotate_left( void ) noexcept;
		float const*	getViewData( void ) noexcept;

		static constexpr float CAMERA_SPEED = 0.75;

	private:
		void	_update( void ) noexcept;

		VectF3 _cameraPos;
		VectF3 _cameraFront;
		VectF3 _cameraUp;
		Matrix4 _view;
};

class ScopGL {
	public:
		ScopGL( void ) noexcept :
			_window(nullptr),
			_texture(0U),
			_shaderProgram(0U),
			_VBO(0U),
			_EBO(0U),
			_VAO(0U),
			_camera(std::make_unique<CameraGL>(VectF3{0.0f, 0.0f, 8.0f}, VectF3{0.0f, 0.0f, -1.0f})),
			_applyTextures(false) {};
		~ScopGL( void );

		void parseFile( std::string const& );
		void createWindow( int32_t, int32_t );
		void initGL( std::string const&, std::string const&, std::string const& );
		void sendBuffersToGPU( void );
		void start( void );

	private:
		GLFWwindow*					_window;
		GLuint						_texture;
		GLuint						_shaderProgram;
		GLuint						_VBO;
		GLuint						_EBO;
		GLuint						_VAO;
		std::shared_ptr<VBO>		_VBOdata;
		std::shared_ptr<EBO>		_EBOdata;

		std::unique_ptr<CameraGL>	_camera;
		GLuint						_currentWidth;
		GLuint						_currentHeight;
		bool						_applyTextures;

		void		_resetWindowCb( uint32_t, uint32_t ) noexcept;
		void		_closeWindowCb( void ) noexcept;
		void		_resetApplyTextures( void );
		void 		_createShader( GLenum type, std::string const&);
		uint32_t	_loadShader( GLenum, std::string const& );
		void 		_loadTexture( std::string const& );
};
