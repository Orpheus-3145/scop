#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "define.hpp"
#include "exceptions.hpp"
#include "parser/parser.hpp"
#include "window/scopWindow.hpp"


int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "wrong parameter format, usage: './scop [file_name]'" << std::endl;
		return(EXIT_FAILURE);
	}

	FileParser parser(argv[argc - 1]);
	std::cout << "parsing file: " << parser.getFileName() << std::endl;

	try {
		parser.parse();
	} catch (const ParsingException& error ) {
		std::cout << "parsing failed: " << error.what() << std::endl;
		return(EXIT_FAILURE);
	}

	ScopWindow window;

	// catch exceptions
	window.open(WINDOW_WIDTH, WINDOW_HEIGHT);

	return (EXIT_SUCCESS);
}