#pragma once
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "exceptions.hpp"
#include "scopGL/scopMath.hpp"


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
		Face( FaceType type, std::vector<VectUI3D> const& coors ) noexcept : _type(type), _indexes(coors), _smoothing(0) {};
		~Face( void ) = default;

		void setObject( std::string const& ) noexcept;
		void setGroup( std::string const& ) noexcept;
		void setMaterial( std::string const& ) noexcept;
		void setSmoothing( uint32_t ) noexcept;

		FaceType 						getFaceType( void ) const noexcept;
		std::vector<VectUI3D> const&	getIndexes( void ) const noexcept;
		std::string 					getObject( void ) const noexcept;
		std::string 					getGroup( void ) const noexcept;
		std::string 					getMaterial( void ) const noexcept;
		uint32_t 						getSmoothing( void ) const noexcept;
		
	private:
		FaceType 				_type;
		std::vector<VectUI3D>	_indexes;
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

	float const* getData( void ) const;
};

struct EBO {
	uint32_t					size;
	uint32_t					stride;
	FaceType					type;
	std::unique_ptr<uint32_t[]>	data;

	uint32_t const*	getData( void ) const;
};

class FileParser;

class ParsedData {
	public:
		~ParsedData( void ) = default;

		std::vector<std::string> const&	getTmlFiles( void ) const noexcept;
		std::vector<VectF3D> const& 	getVertices( void ) const noexcept;
		std::vector<VectF2D> const& 	getTextures( void ) const noexcept;
		std::vector<VectF3D> const& 	getVerticesNorm( void ) const noexcept;
		std::vector<VectF3D> const&		getParamSpaceVertices( void ) const noexcept;
		std::vector<Face> const& 		getFaces( void ) const noexcept;
		std::vector<Line> const& 		getLines( void ) const noexcept;
		std::shared_ptr<VBO> const&		getVBO( void ) const;
		std::shared_ptr<EBO> const&		getEBO( void ) const;
		bool							hasFaces( void ) const noexcept;

		// reference: https://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf
		void fillBuffers( void );

		// 44 bytes in total: (3floats vertex + 3floats color + 2floats texture + 3floats normal) * 4bytes
		static constexpr uint32_t VBO_STRIDE = sizeof(VectF3D) /*vertex*/ + sizeof(VectF3D) /*color*/ + sizeof(VectF2D) /*texture*/ + sizeof(VectF3D) /*normal*/;
		static constexpr uint32_t EBO_STRIDE = sizeof(uint32_t);
		
		friend class FileParser;

	private:
		ParsedData( void ) = default;

		std::vector<std::byte>							_serializeVertex( VectUI3D const&, FaceType ) const noexcept;
		void											_fillVBOnoFaces( void );
		std::vector<std::vector<VectUI3D>>				_earClip( std::vector<VectUI3D> const& ) const noexcept;
		std::list<std::pair<VectUI3D,VectF2D>>			_create2Dvertexes( std::vector<VectUI3D> const& ) const noexcept;
		bool											_isConvex( std::list<std::pair<VectUI3D,VectF2D>>::const_iterator const&, std::list<std::pair<VectUI3D,VectF2D>> const& ) const noexcept;
		bool											_isEar( std::list<std::pair<VectUI3D,VectF2D>>::const_iterator const&, std::list<std::pair<VectUI3D,VectF2D>> const& ) const noexcept;
		std::array<std::byte,ParsedData::VBO_STRIDE>	_serialize( VectUI3D const&, FaceType ) const noexcept;
		
		std::vector<std::string> 	_tmlFiles;
		std::vector<VectF3D> 		_vertexes;
		std::vector<VectF2D> 		_textures;
		std::vector<VectF3D> 		_vertexNorms;
		std::vector<VectF3D> 		_paramSpaceVertices;
		std::vector<Face>			_faces;
		std::vector<Line> 			_lines;
		std::shared_ptr<VBO>		_VBOdata;
		std::shared_ptr<EBO>		_EBOdata;

};

class FileParser {
	public:
		FileParser( void ) noexcept : _currentSmoothing(0) {}; 
		~FileParser( void ) = default;
		// reference https://en.wikipedia.org/wiki/Wavefront_.obj_file
		ParsedData	parse( std::string const& );

	private:
		void		_parseDirective( std::string const&, ParsedData& );
		std::string _createFile( std::string const& ) const;
		VectF3D 	_createVertex( std::string const& ) const;
		VectF2D 	_createTexture( std::string const& ) const;
		VectF3D 	_createVertexNorm( std::string const& ) const;
		VectF3D		_createSpaceVertex( std::string const& ) const;
		Face 		_createFace( std::string const& ) const;
		Line 		_createLine( std::string const& ) const;

		std::string _trimString( std::string const& ) const;
		float		_parseFloat( std::string const& ) const;
		int32_t		_parseInt( std::string const& ) const;
		uint32_t	_parseUint( std::string const& ) const;

		std::string _currentObject;
		std::string _currentGroup;
		int32_t		_currentSmoothing;
		std::string _currentMaterial;
};

std::ostream& operator<<( std::ostream&, FaceType );
std::ostream& operator<<( std::ostream&, Face const& );
std::ostream& operator<<( std::ostream&, Line const& );
std::ostream& operator<<( std::ostream&, VBO const& );
std::ostream& operator<<( std::ostream&, EBO const& );
std::ostream& operator<<( std::ostream&, ParsedData const& );
