#include <iostream>

#include "exception.hpp"
#include "scop.hpp"
#include "argParser.hpp"


int32_t main(int32_t argc, char** argv) {
	try {
		InputData options = InputData::parseArgs(argc, argv);

		if (options.helpmode == true) {
			std::cout << HOW_TO << std::endl;
			return (EXIT_SUCCESS);
		}
		
		ScopGL app = ScopGL();
		app.parseFile(options.objFile);
		app.createWindow(options.width, options.height);
		app.initGL(options.vertexShaderFile, options.fragmentShaderFile, options.textureFile);
		app.loop();

	} catch (AppException& err) {
		std::cerr << err.what() << std::endl;
		return(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}