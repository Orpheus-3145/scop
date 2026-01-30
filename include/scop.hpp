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
		CameraGL( GLuint shader, VectF3 const& pos, VectF3 const& front, std::string const& uniformName = "view" ) :
			GraphicGL(shader, uniformName),
			_cameraPos(pos),
			_cameraTarget(front),
			_cameraUp(VectF3{0.0f, 1.0f, 0.0f}) {};

		void	moveForward( void ) noexcept;
		void	moveBackward( void ) noexcept;
		void	rotateRight( void ) noexcept;
		void	rotateLeft( void ) noexcept;
		void	updateShader( void ) override;

	protected:
		VectF3	_cameraPos;
		VectF3	_cameraTarget;
		VectF3	_cameraUp;
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
