#include <iostream>

#include "exceptions.hpp"
#include "scopGL/scopGL.hpp"


int32_t main(int32_t argc, char** argv) {
	if (argc != 2) {
		std::cout << "wrong parameter format, usage: './scop [file_obj]'" << std::endl;
		return(EXIT_FAILURE);
	}

	try {
		ScopGL app = ScopGL();
	
		app.parseFile(argv[argc - 1]);
		
		app.createWindow(SCOP_WINDOW_WIDTH, SCOP_WINDOW_HEIGHT);
		app.initGL(SCOP_VERTEX_SHADER, SCOP_FRAGMENT_SHADER, SCOP_TEXTURE_CAPYBARA);
		app.sendBuffersToGPU();
		app.start();
		
	} catch (AppException const& err) {
		std::cerr << "Error: " << err.what() << std::endl;
		return(EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}