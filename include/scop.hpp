#pragma once
#include <cstdint>
#include <memory>
#include <GLFW/glfw3.h>

#include "math/matrix.hpp"
#include "define.hpp"
#include "parser.hpp"
#include "data.hpp"


class GraphicGL {
	public:
		GraphicGL( GLuint, std::string const& uniformName = "transformation" );
		virtual ~GraphicGL( void ) = default;
		
		virtual void	updateShader( void );
		void			reset( void ) noexcept;
		
	protected:
		GLuint	_shader;
		GLint	_shaderLocation;
		Matrix4 _transformation;

};

class ModelGL : public GraphicGL {
	public:
		ModelGL(GLuint shader, std::string const& uniformName = "model") : 
			GraphicGL(shader, uniformName) {};

		void	rotate( float, VectF3 const& ) noexcept;
		void	translate( VectF3 const& ) noexcept;
		void	scale( VectF3 const& ) noexcept;

		void	updateShader( void ) override;
};


class CameraGL : public GraphicGL{
	public:
		CameraGL( GLuint shader, VectF3 const& pos, std::string const& uniformName = "view" ) :
			GraphicGL(shader, uniformName),
			_position(pos),
			_forward(VectF3{0.0f, 0.0f, -SCOP_CAMERA_DISTANCE}),
			__up(VectF3{0.0f, 1.0f, 0.0f}) {
				this->updateShader();
			};

		void	moveForward( float ) noexcept;
		void	moveBackward( float ) noexcept;
		void	moveRight( float ) noexcept;
		void	moveLeft( float ) noexcept;
		void	rotate( float, float, float ) noexcept;
		void	updateShader( void ) override;

	protected:
		VectF3	_position;		// position of the camera
		VectF3	_forward;		// where the camera is watching
		VectF3	__up;			// general up
		VectF3	_cameraForward;	// z axis of the camera
		VectF3	_cameraLeft;	// x axis of the camera
		VectF3	_cameraUp;		// y axis of the camera
};


class ProjectionGL : public GraphicGL{
	public:
		ProjectionGL( GLuint shader, uint32_t width, uint32_t height, std::string const& uniformName = "projection" ) :
			GraphicGL(shader, uniformName),
			_aspect(static_cast<float>(width) / static_cast<float>(height)) {};

		void	setAspect( uint32_t, uint32_t ) noexcept;
		void	updateShader( void ) override;

	protected:
		float	_aspect;
};


class ScopGL {
	public:
		ScopGL( void ) noexcept;
		~ScopGL( void ) noexcept;

		void parseFile( std::string const& );
		void createWindow( int32_t, int32_t );
		void initGL( std::string const&, std::string const&, std::string const& );
		void loop( void );

		void resetCanvaSize( uint32_t, uint32_t );
		void closeWindow( void );

	private:
		GLFWwindow*					_window;
		uint32_t					_widthWindow;
		uint32_t					_heightWindow;
		GLuint						_texture;
		GLuint						_shaderProgram;
		GLuint						_VBO;
		GLuint						_EBO;
		GLuint						_VAO;
		std::shared_ptr<VBO>		_VBOdata;
		std::shared_ptr<EBO>		_EBOdata;

		float	_currCursorX;
		float	_currCursorY;

		std::unique_ptr<ModelGL>		_model;
		std::unique_ptr<CameraGL>		_camera;
		std::unique_ptr<ProjectionGL>	_projection;
		bool							_applyTextures;

		void 		_createShader( GLenum type, std::string const&);
		uint32_t	_loadShader( GLenum, std::string const& );
		void 		_loadTexture( std::string const& );
		void		_setupCallbacks( void );
		void		_loadBuffersInGPU( void );
		void		_moveCamera( void );
		void		_centerCursor( void );
		void		_toggleTextures( void );
		void		_rotateCamera( float, float );
};
