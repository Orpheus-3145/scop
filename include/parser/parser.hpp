#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>

#include "exceptions.hpp"
#include "parser/parserTypes.hpp"


// reference https://en.wikipedia.org/wiki/Wavefront_.obj_file
class FileParser {
	public:
		FileParser( void ) noexcept : _currentSmoothing(0) {}; 
		~FileParser( void ) noexcept {};

		std::unique_ptr<ParsedData>	parse( std::string const& );

	private:
		std::string _fileName;
		std::string _currentObject;
		std::string _currentGroup;
		int			_currentSmoothing;
		std::string _currentMaterial;

		std::string 			_createFile( std::string const& ) const;
		VertexCoor 				_createVertex( std::string const& ) const;
		TextureCoor 			_createTexture( std::string const& ) const;
		VertexNormCoor 			_createVertexNorm( std::string const& ) const;
		VertexSpaceParamCoor	_createSpaceVertex( std::string const& ) const;
		Face 					_createFace( std::string const& ) const;
		Line 					_createLine( std::string const& ) const;

		void _setObject( std::string const& );
		void _setGroup( std::string const& );
		void _setMaterial( std::string const& );
		void _setSmoothing( std::string const& );

		double			_parseDouble( std::string const& ) const;
		int 			_parseInt( std::string const& ) const;
		unsigned int 	_parseUint( std::string const& ) const;
};
