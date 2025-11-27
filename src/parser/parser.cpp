#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "parser/parser.hpp"
#include "parser/parserTypes.hpp"
#include "exceptions.hpp"


void FileParser::parse( void ) {
	std::ifstream streamFile(this->fileName);
	if (!streamFile)
		throw ParsingException("Error while opening file: " + this->fileName);
	
    std::string directive;
    while (std::getline(streamFile, directive)) {
        std::cout << directive << "\n";
    }

	streamFile.close();
}

std::string const& FileParser::getFileName( void ) const noexcept {
	return this->fileName;
}