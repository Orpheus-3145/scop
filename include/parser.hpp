#pragma once
#include <vector>
#include <array>
#include <list>
#include <string>
#include <iostream>
#include <memory>
#include <cstdint>

#include "math/vector.hpp"


enum FaceType {
	VERTEX,
	VERTEX_TEXT,
	VERTEX_VNORM,
	VERTEX_TEXT_VNORM
};

std::string	faceToString( FaceType );

class Face {
	public:
		Face( void ) = default;
		explicit Face( FaceType type ) noexcept : _type(type), _smoothing(0) {};
		Face( FaceType type, std::vector<VectUI3> const& coors ) noexcept : _type(type), _indexes(coors), _smoothing(0) {};
		~Face( void ) = default;

		void setFaceType( FaceType ) noexcept;
		void setIndexes( std::vector<VectUI3> const& ) noexcept;
		void setObject( std::string const& ) noexcept;
		void setGroup( std::string const& ) noexcept;
		void setMaterial( std::string const& ) noexcept;
		void setSmoothing( uint32_t ) noexcept;

		FaceType 						getFaceType( void ) const noexcept;
		std::vector<VectUI3> const&		getIndexes( void ) const noexcept;
		std::string 					getObject( void ) const noexcept;
		std::string 					getGroup( void ) const noexcept;
		std::string 					getMaterial( void ) const noexcept;
		uint32_t 						getSmoothing( void ) const noexcept;
		
	private:
		FaceType 				_type;
		std::vector<VectUI3>	_indexes;
		std::string 			_object;
		std::string 			_group;
		std::string 			_material;
		uint32_t 				_smoothing;
};

class Line {
	public:
		Line( void ) = default;
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

// 44 bytes in total: (3floats vertex + 3floats color + 2floats texture + 3floats normal) * 4bytes
static constexpr uint32_t VBO_STRIDE = sizeof(VectF3) /*vertex*/ + sizeof(VectF3) /*color*/ + sizeof(VectF2) /*texture*/ + sizeof(VectF3) /*normal*/;
// 32 bytes: the RGB is not stored
static constexpr uint32_t VERTEX_STRIDE = VBO_STRIDE - sizeof(VectF3);
static constexpr uint32_t EBO_STRIDE = sizeof(uint32_t);
using SerializedVertex = std::array<std::byte,VERTEX_STRIDE>;

struct VectorByteHash {
	uint32_t operator()(SerializedVertex const& arr) const noexcept {
		uint32_t h = 0U, magicNo = 0x9e3779b9;
		for (auto b : arr)
			h ^= std::to_integer<uint32_t>(b) + magicNo + (h << 6) + (h >> 2);
		return h;
	}
};

struct VectorByteEqual {
	bool operator()(SerializedVertex const& a, SerializedVertex const& b) const noexcept {
		return a == b;
	}
};

class FileParser;

class ParsedData {
	public:
		~ParsedData( void ) = default;

		std::vector<std::string> const&	getTmlFiles( void ) const noexcept;
		std::vector<VectF3> const&		getVertices( void ) const noexcept;
		std::vector<VectF2> const&		getTextures( void ) const noexcept;
		std::vector<VectF3> const&		getVerticesNorm( void ) const noexcept;
		std::vector<VectF3> const&		getParamSpaceVertices( void ) const noexcept;
		std::list<Face> const&	 		getFaces( void ) const noexcept;
		std::list<Line> const&	 		getLines( void ) const noexcept;
		std::shared_ptr<VBO> const&		getVBO( void ) const;
		std::shared_ptr<EBO> const&		getEBO( void ) const;
		bool							hasFaces( void ) const noexcept;

		void	triangolate( void );
		void	fixTrianglesOrientation( void );
		void	fillTexturesAndNormals( void );
		// reference: https://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf
		void	fillBuffers( void );
		void	fillVBOnoFaces( void );
		
		friend class FileParser;

	private:
		ParsedData( void ) = default;

		std::vector<VectUI3>					_spawnTriangle( std::list<std::pair<VectUI3,VectF2>>&, std::list<VectF2>&, std::list<VectF2>&, std::list<VectF2>& ) const noexcept;
		std::list<std::pair<VectUI3,VectF2>>	_create2Dvertexes( std::vector<VectUI3> const& ) const noexcept;
		bool									_isConvex( std::list<std::pair<VectUI3,VectF2>>::const_iterator const&, std::list<std::pair<VectUI3,VectF2>> const& ) const noexcept;
		bool									_isEar( std::list<std::pair<VectUI3,VectF2>>::const_iterator const&, std::list<std::pair<VectUI3,VectF2>> const& ) const noexcept;
		SerializedVertex						_serializeVertex( VectUI3 const&, FaceType ) const;
		
		std::vector<std::string>	_tmlFiles;
		std::vector<VectF3> 		_vertexes;
		std::vector<VectF2> 		_textures;
		std::vector<VectF3> 		_normals;
		std::vector<VectF3> 		_paramSpaceVertices;
		std::list<Face>				_faces;
		std::list<Line> 			_lines;
		std::shared_ptr<VBO>		_VBOdata;
		std::shared_ptr<EBO>		_EBOdata;
		bool						_triangolationDone = false;
		bool						_dataFilled = false;
};

std::ostream& operator<<( std::ostream&, FaceType );
std::ostream& operator<<( std::ostream&, Face const& );
std::ostream& operator<<( std::ostream&, Line const& );
std::ostream& operator<<( std::ostream&, VBO const& );
std::ostream& operator<<( std::ostream&, EBO const& );
std::ostream& operator<<( std::ostream&, ParsedData const& );

class FileParser {
	public:
		FileParser( void ) noexcept : _currentSmoothing(0) {}; 
		~FileParser( void ) = default;
		// reference https://en.wikipedia.org/wiki/Wavefront_.obj_file
		ParsedData	parse( std::string const& );

	private:
		void		_parseDirective( std::string const&, ParsedData& );
		std::string _createFile( std::string const& ) const;
		VectF3 		_createVertex( std::string const& ) const;
		VectF2 		_createTexture( std::string const& ) const;
		VectF3 		_createVertexNorm( std::string const& ) const;
		VectF3		_createSpaceVertex( std::string const& ) const;
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
