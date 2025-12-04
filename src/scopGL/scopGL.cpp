#include "scopGL/scopGL.hpp"


void ScopGL::parseFile( std::string const& fileName ) {
	std::cout << "parsing file: " << fileName << std::endl;
	this->_parser.parse(fileName);
}

void ScopGL::createWindow( void ) {
	std::cout << "opening window: " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
	this->_window.init(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void ScopGL::start( void ) {
	this->_window.startLoop();
}

void ScopGL::loadShader(int type, std::string const& fileName) {
	std::ifstream readFile;

	readFile.open(fileName, std::ifstream::in);
	if (!readFile)
		throw WindowException("Failed to open shader file: " + fileName);

	std::stringstream buffer;
	buffer << readFile.rdbuf();
	readFile.close();

	std::string fileContentStr = buffer.str();
	const char* fileContent = fileContentStr.c_str();
	unsigned int shaderRef = glCreateShader(type);
	glShaderSource(shaderRef, 1, &fileContent, NULL);
	glCompileShader(shaderRef);

	int  success;
	char infoLog[512];
	glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shaderRef, 512, NULL, infoLog);
		throw("Failed to compile shader: " + fileName + ", trace: " + infoLog);
	}
}
