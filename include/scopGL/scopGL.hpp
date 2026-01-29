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


class ModelGL {
	public:
		ModelGL( void ) noexcept : _model(idMat()), _transpose(true) {};
		~ModelGL( void ) = default;

		void			rotate( float, VectF3 const& ) noexcept;
		void			translate( VectF3 const& ) noexcept;
		void			scale( VectF3 const& ) noexcept;
		float const*	getModelData( void ) noexcept;

	private:
		void	_update( void ) noexcept;

		Matrix4 _model;
		bool	_transpose;
};


class CameraGL {
	public:
		CameraGL( VectF3 const& pos, VectF3 const& front ) noexcept :
			_cameraPos(pos), 
		  	_cameraFront(front), 
		  	_cameraUp(VectF3{0.0f, 1.0f, 0.0f}),
			_transpose(true) {
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

		VectF3	_cameraPos;
		VectF3	_cameraFront;
		VectF3	_cameraUp;
		Matrix4 _view;
		bool	_transpose;
};


class ProjectionGL {
	public:
		ProjectionGL( uint32_t width, uint32_t height ) noexcept :
			_transpose(true) {
				this->updateAspect(width, height);
		};
		~ProjectionGL( void ) = default;

		void			updateAspect( uint32_t, uint32_t ) noexcept;
		float const*	getProjectionData( void ) noexcept;

		static constexpr float	PRJ_FOV = 45.0f;
		static constexpr float	PRJ_NEAR = 1.0f;
		static constexpr float	PRJ_FAR = 100.0f;
		static constexpr bool	PRJ_FINITE = true;

	private:
		void	_update( void ) noexcept;

		float	_aspect;
		Matrix4	_projection;
		bool	_transpose;
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
