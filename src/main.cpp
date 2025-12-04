#include <iostream>

#include "exceptions.hpp"
#include "scopGL/scopGL.hpp"


int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "wrong parameter format, usage: './scop [file_obj]'" << std::endl;
		return(EXIT_FAILURE);
	}

	ScopGL app = ScopGL();
	app.parseFile(argv[argc - 1]);

	app.createWindow();

	app.loadShader(GL_VERTEX_SHADER, "resources/shaders/vertexShaderTest.glsl");
	app.loadShader(GL_FRAGMENT_SHADER, "resources/shaders/fragmentShaderTest.glsl");

	app.start();
	// FileParser parser();
	// try {
	// 	parser.parse(argv[argc - 1]);
	// } catch (const ParsingException& error ) {
	// 	std::cout << "parsing failed: " << error.what() << std::endl;
	// 	return(EXIT_FAILURE);
	// }

	// ScopWindow window = ScopWindow();
	// window.open(WINDOW_WIDTH, WINDOW_HEIGHT);

	return (EXIT_SUCCESS);
}