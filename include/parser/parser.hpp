#pragma once
#include <string>
#include <sstream>
#include "parser/parserTypes.hpp"

// reference https://en.wikipedia.org/wiki/Wavefront_.obj_file
class FileParser {
	public:
		FileParser( std::string const& fileName) noexcept;
		~FileParser( void ) noexcept;

		void				parse( void );
		std::string const&	getFileName( void ) const noexcept;
		ObjData const& 		getData( void ) const;
		void				showData( void ) const;

	private:
		std::string _fileName;
		ObjData		*_data;
		std::string _currentObject;
		std::string _currentGroup;
		int			_currentSmoothing;
		std::string _currentMaterial;

		void _addFile( std::string const& );
		void _addVertex( std::string const& );
		void _addTexture( std::string const& );
		void _addVertexNorm( std::string const& );
		void _addSpaceVertex( std::string const& );
		void _addFace( std::string const& );
		void _addLine( std::string const& );
		void _setObject( std::string const& );
		void _setGroup( std::string const& );
		void _setMaterial( std::string const& );
		void _setSmoothing( std::string const& );

		double	_parseDouble( std::string const& ) const;
		int 	_parseInt( std::string const& ) const;
};
