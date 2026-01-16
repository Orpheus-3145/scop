#pragma once
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <cstddef>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <cstring>

#include "exceptions.hpp"
#include "scopGL/scopMath.hpp"


struct coor2D {
	float x;
	float y;

	coor2D( void ) noexcept						= default;
	coor2D( coor2D const& ) noexcept			= default;
	coor2D& operator=( coor2D const& ) noexcept	= default;
	~coor2D( void )								= default;

	static coor2D from_vector( std::vector<float> const& ) noexcept;
};

struct coor3D {
	float x;
	float y;
	float z;

	coor3D( void ) noexcept						= default;
	coor3D( coor3D const& ) noexcept			= default;
	coor3D& operator=( coor3D const& ) noexcept	= default;
	~coor3D( void )								= default;

	static coor3D from_vector( std::vector<float> const& );
};

struct index3D {
	uint32_t i1;
	uint32_t i2;
	uint32_t i3;

	index3D( void ) noexcept 						= default;
	index3D( index3D const& ) noexcept				= default;
	index3D& operator=( index3D const& ) noexcept	= default;
	~index3D( void ) 								= default;

	static index3D from_vector( std::vector<uint32_t> const& ) noexcept;
};

enum FaceType {
	VERTEX,
	VERTEX_TEXT,
	VERTEX_VNORM,
	VERTEX_TEXT_VNORM
};

std::string	faceToString( FaceType );

class Face {
	public:
		explicit Face( FaceType type ) noexcept : _type(type), _smoothing(0) {};
		Face( FaceType type, std::vector<index3D> const& coors ) noexcept : _type(type), _indexes(coors), _smoothing(0) {};
		~Face( void ) = default;

		void setObject( std::string const& ) noexcept;
		void setGroup( std::string const& ) noexcept;
		void setMaterial( std::string const& ) noexcept;
		void setSmoothing( uint32_t ) noexcept;

		FaceType 					getFaceType( void ) const noexcept;
		std::vector<index3D> const&	getIndexes( void ) const noexcept;
		std::string 				getObject( void ) const noexcept;
		std::string 				getGroup( void ) const noexcept;
		std::string 				getMaterial( void ) const noexcept;
		uint32_t 					getSmoothing( void ) const noexcept;

	private:
		FaceType 				_type;
		std::vector<index3D>	_indexes;
		std::string 			_object;
		std::string 			_group;
		std::string 			_material;
		uint32_t 				_smoothing;
};

class Line {
	public:
		explicit Line(std::vector<uint32_t> const& coors ) noexcept : _indexes(coors), _smoothing(0) {};
		~Line( void ) = default;

		void setObject( std::string const& ) noexcept;
		void setGroup( std::string const& ) noexcept;
		void setMaterial( std::string const& ) noexcept;
		void setSmoothing( uint32_t ) noexcept;

		std::vector<uint32_t> const&	getIndexes( void ) const noexcept;
		std::string 					getObject( void ) const noexcept;
		std::string 					getGroup( void ) const noexcept;
		std::string 					getMaterial( void ) const noexcept;
		uint32_t 						getSmoothing( void ) const noexcept;

	private:
		std::vector<uint32_t>	_indexes;
		std::string 			_object;
		std::string 			_group;
		std::string 			_material;
		uint32_t				_smoothing;
};

struct VBO {
	uint32_t					size;
	uint32_t					stride;
	std::unique_ptr<float[]>	data;

	float const*	getData( void ) const;
};

struct EBO {
	uint32_t					size;
	uint32_t					stride;
	FaceType					type;
	std::unique_ptr<uint32_t[]>	data;

	uint32_t const*	getData( void ) const;
};

class ParsedData {
	public:
		ParsedData( void )	= default;
		~ParsedData( void ) = default;

		std::vector<std::string> const&	getTmlFiles( void ) const noexcept;
		std::vector<coor3D> const& 		getVertices( void ) const noexcept;
		std::vector<coor2D> const& 		getTextures( void ) const noexcept;
		std::vector<coor3D> const& 		getVerticesNorm( void ) const noexcept;
		std::vector<coor3D> const&		getParamSpaceVertices( void ) const noexcept;
		std::vector<Face> const& 		getFaces( void ) const noexcept;
		std::vector<Line> const& 		getLines( void ) const noexcept;
		std::shared_ptr<VBO> const&		getVBO( void ) const;
		std::shared_ptr<EBO> const&		getEBO( void ) const;

		void addTmlFile( std::string const& ) noexcept;
		void addVertex( coor3D const& ) noexcept;
		void addTexture( coor2D const& ) noexcept;
		void addVertexNorm( coor3D const& ) noexcept;
		void addParamSpaceVertex( coor3D const& ) noexcept;
		void addFace( Face const& );
		void addLine( Line const& ) noexcept;

		void createBuffers( void ) noexcept;
		// std::shared_ptr<VBO>	createVBO( void ) const noexcept;
		// std::shared_ptr<EBO>	createEBO( FaceType ) const;

	private:
		std::vector<std::string> 	_tmlFiles;
		std::vector<coor3D> 		_vertexes;
		std::vector<coor2D> 		_textures;
		std::vector<coor3D> 		_vertexNorms;
		std::vector<coor3D> 		_paramSpaceVertices;
		std::vector<Face>			_faces;
		std::vector<Line> 			_lines;
		std::shared_ptr<VBO>		_VBOdata;
		std::shared_ptr<EBO>		_EBOdata;
};

std::ostream& operator<<( std::ostream&, coor2D const& );
std::ostream& operator<<( std::ostream&, coor3D const& );
std::ostream& operator<<( std::ostream&, index3D const& );
std::ostream& operator<<( std::ostream&, FaceType );
std::ostream& operator<<( std::ostream&, Face const& );
std::ostream& operator<<( std::ostream&, Line const& );
std::ostream& operator<<( std::ostream&, VBO const& );
std::ostream& operator<<( std::ostream&, EBO const& );
std::ostream& operator<<( std::ostream&, ParsedData const& );
