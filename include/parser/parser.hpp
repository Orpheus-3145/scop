#pragma once
#include <string>
#include "parser/parserTypes.hpp"

class FileParser {
	private:
		std::string fileName;
		ObjData		data;

	public:
		FileParser( std::string const& fileName) : fileName(fileName) {};
		~FileParser( void ) noexcept {};
		void parse( void );
		std::string const& getFileName( void ) const noexcept;
};
