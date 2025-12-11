#include <iostream>

#include "exceptions.hpp"
#include "scopGL/scopGL.hpp"


int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "wrong parameter format, usage: './scop [file_obj]'" << std::endl;
		return(EXIT_FAILURE);
	}

	try {
		ScopGL app = ScopGL();
	
		app.parseFile(argv[argc - 1]);
		app.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	
		app.loadData();
		app.start();
		
	} catch (AppException const& err) {
		std::cerr << "Error: " << err.what() << std::endl;
		return(EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}