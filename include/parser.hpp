#pragma once
#include <vector>
#include <array>
#include <list>
#include <string>
#include <iostream>
#include <memory>
#include <cstdint>
#include <filesystem>

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

std::ostream& operator<<( std::ostream&, FaceType );
std::ostream& operator<<( std::ostream&, Face const& );
std::ostream& operator<<( std::ostream&, Line const& );

class ParsedData;

namespace fs = std::filesystem;

class FileParser {
	public:
		FileParser( void ) noexcept : _currentSmoothing(0) {}; 
		~FileParser( void ) = default;
		// reference https://en.wikipedia.org/wiki/Wavefront_.obj_file
		ParsedData	parse( std::string const& );

	private:
		void		_parseDirective( std::string const&, ParsedData& );
		fs::path _createFile( std::string const& ) const;
		VectF3 		_createVertex( std::string const& ) const;
		VectF2 		_createTexture( std::string const& ) const;
		VectF3 		_createVertexNorm( std::string const& ) const;
		VectF3		_createSpaceVertex( std::string const& ) const;
		Face 		_createFace( std::string const& ) const;
		Line 		_createLine( std::string const& ) const;

		std::string _trimString( std::string const& ) const noexcept;
		FaceType	_getFaceType( std::string const& ) const noexcept;
		float		_parseFloat( std::string const& ) const;
		int32_t		_parseInt( std::string const& ) const;
		uint32_t	_parseUint( std::string const& ) const;

		fs::path	_objFile;
		std::string				_currentObject;
		std::string				_currentGroup;
		int32_t					_currentSmoothing;
		std::string				_currentMaterial;
};
