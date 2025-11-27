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
		ObjData const& parse( void );
};
