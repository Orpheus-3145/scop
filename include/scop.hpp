#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "exceptions.hpp"
#include "parser.hpp"
#include "math.hpp"
#include "define.hpp"


class ModelGL {
	public:
		ModelGL( GLuint );
		~ModelGL( void ) = default;

		void	rotate( float, VectF3 const&, bool = true ) noexcept;
		void	translate( VectF3 const&, bool = true ) noexcept;
		void	scale( VectF3 const&, bool = true ) noexcept;

	private:
		void	_update( void ) noexcept;

		GLuint	_shader;
		GLint	_shaderLocation;
		Matrix4 _model;
};


class CameraGL {
	public:
		CameraGL( GLuint, VectF3 const&, VectF3 const& );
		~CameraGL( void ) = default;

		void	move_forward( void ) noexcept;
		void	move_backward( void ) noexcept;
		void	rotate_right( void ) noexcept;
		void	rotate_left( void ) noexcept;

	private:
		void	_update( void ) noexcept;

		GLuint	_shader;
		GLint	_shaderLocation;
		VectF3	_cameraPos;
		VectF3	_cameraFront;
		VectF3	_cameraUp;
		Matrix4 _view;
};


class ProjectionGL {
	public:
		ProjectionGL( GLuint, uint32_t, uint32_t );
		~ProjectionGL( void ) = default;

		void	updateAspect( uint32_t, uint32_t ) noexcept;

	private:
		void	_update( void ) noexcept;

		GLuint	_shader;
		GLint	_shaderLocation;
		float	_aspect;
		Matrix4	_projection;
};


class ScopGL {
	public:
		ScopGL( void ) noexcept;
		~ScopGL( void ) noexcept;

		void parseFile( std::string const& );
		void createWindow( int32_t, int32_t );
		void initGL( std::string const&, std::string const&, std::string const& );
		void loop( void );

	private:
		GLFWwindow*					_window;
		GLuint						_texture;
		GLuint						_shaderProgram;
		GLuint						_VBO;
		GLuint						_EBO;
		GLuint						_VAO;
		std::shared_ptr<VBO>		_VBOdata;
		std::shared_ptr<EBO>		_EBOdata;

		std::unique_ptr<ModelGL>		_model;
		std::unique_ptr<CameraGL>		_camera;
		std::unique_ptr<ProjectionGL>	_projection;
		bool							_applyTextures;

		void 		_createShader( GLenum type, std::string const&);
		uint32_t	_loadShader( GLenum, std::string const& );
		void 		_loadTexture( std::string const& );
		void		_setupCallbacks( void );
		void		_loadBuffersInGPU( void );
		// callbacks
		void		_resetWindowSize( uint32_t, uint32_t );
		void		_closeWindow( void );
		void		_toggleTextures( void );
};
