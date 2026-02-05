#pragma once
#include <cstdint>
#include <memory>
#include <iostream>
#include <vector>
#include <list>

#include "parser.hpp"
#include "math/vector.hpp"


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

		// reference: https://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf
		void	triangolate( void );
		void	fixTrianglesOrientation( void );
		void	fillTexturesAndNormals( void );
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

std::ostream& operator<<( std::ostream&, VBO const& );
std::ostream& operator<<( std::ostream&, EBO const& );
std::ostream& operator<<( std::ostream&, ParsedData const& );
