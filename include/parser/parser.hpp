#pragma once
#include <string>
#include <sstream>
#include "parser/parserTypes.hpp"

class FileParser {
	private:
		std::string _fileName;
		ObjData		_data;
		bool		_dataParsed = false;
		std::string _currentObject;
		std::string _currentGroup;
		int			_currentSmoothing;
		std::string _currentMaterial;

		void _addFile( std::stringstream& );
		void _addVertex( std::stringstream& );
		void _addTexture( std::stringstream& );
		void _addVertexNorm( std::stringstream& );
		void _addSpaceVertex( std::stringstream& );
		void _addFace( std::stringstream& );
		void _addLine( std::stringstream& );
		void _setObject( std::stringstream& );
		void _setGroup( std::stringstream& );
		void _setMaterial( std::stringstream& );
		void _setSmoothing( std::stringstream& );

		double	_parseDouble( std::string const& ) const;
		int 	_parseInt( std::string const& ) const;

	public:
		FileParser( std::string const& fileName) noexcept : _fileName(fileName) {};
		~FileParser( void ) noexcept {};
		void parse( void );
		std::string const& getFileName( void ) const noexcept;
		ObjData const& getData( void ) const;
};
