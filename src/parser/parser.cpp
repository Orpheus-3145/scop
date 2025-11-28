#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>

#include "parser/parser.hpp"
#include "parser/parserTypes.hpp"
#include "exceptions.hpp"


void FileParser::_addFile( std::stringstream& ss ) {
	std::string fileName;
	if (ss >> fileName)
		throw ParsingException("Invalid file name");
	this->_data.addTmlFile(fileName);
}

void FileParser::_addVertex( std::stringstream& ss ) {
	std::vector<double> coorList;
	std::string coor;

	for (int i=0;i<3;i++) {
		if (!(ss >> coor))
			throw ParsingException("Invalid vertex coordinate");
		coorList.push_back(this->_parseDouble(coor));
	}
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));

	this->_data.addVertex(coorList);
}

void FileParser::_addTexture( std::stringstream& ss ) {
	std::vector<double> coorList;
	std::string coor;
	if (!(ss >> coor))
		throw ParsingException("Invalid texture coordinate");
	coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));

	this->_data.addTextureCoor(coorList);
}

void FileParser::_addVertexNorm( std::stringstream& ss ) {
	std::vector<double> coorList;
	std::string coor;

	for (int i=0;i<3;i++) {
		if (!(ss >> coor))
			throw ParsingException("Invalid vertexNorm coordinate");
		coorList.push_back(this->_parseDouble(coor));
	}

	this->_data.addVertexNorm(coorList);
}

void FileParser::_addSpaceVertex( std::stringstream& ss ) {
	std::vector<double> coorList;
	std::string coor;
	if (!(ss >> coor))
		throw ParsingException("Invalid paramSpaceVertex coordinate");
	coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));

	this->_data.addParamSpaceVertex(coorList);
}

void FileParser::_addFace( std::stringstream& ss ) {
	std::array<FaceCoor, 3> indexList;
	std::string index;

	for (int i=0; i<3; i++) {
		if (!(ss >> index))
			throw ParsingException("Invalid face coordinate");
		std::vector<unsigned int> coorList;
		std::stringstream ssCoor(index);
		std::string strNumber;
		while(std::getline(ss, strNumber, '/'))		// NB check '/' splitting
			coorList.push_back(this->_parseInt(strNumber));
		indexList[i] = FaceCoor(coorList);
	}

	FaceType _type;
	size_t firstSlashPos, secondSlashPos;
	firstSlashPos = index.find("/");
	if (firstSlashPos == std::string::npos)
		_type = VERTEX;
	else {
		secondSlashPos = index.find("/", firstSlashPos + 1);
		if (secondSlashPos == firstSlashPos + 1)
			_type = VERTEX_VNORM;
		else if (secondSlashPos == std::string::npos)
			_type = VERTEX_TEXT;
		else
			_type = VERTEX_TEXT_VNORM;
	}

	Face newFace(_type, indexList);
	if (this->_currentObject != "")
		newFace.setObject(this->_currentObject);
	if (this->_currentGroup != "")
		newFace.setGroup(this->_currentGroup);
	if (this->_currentMaterial != "")
		newFace.setMaterial(this->_currentMaterial);
	if (this->_currentSmoothing != -1)
		newFace.setSmoothing(this->_currentSmoothing);
	this->_data.addFace(newFace);
}

void FileParser::_addLine( std::stringstream& ss ) {
	std::vector<unsigned int> indexList;
	std::string index;

	while (ss >> index)
		indexList.push_back(this->_parseInt(index));

	Line newLine(indexList);
	if (this->_currentObject != "")
		newLine.setObject(this->_currentObject);
	if (this->_currentGroup != "")
		newLine.setGroup(this->_currentGroup);
	if (this->_currentMaterial != "")
		newLine.setMaterial(this->_currentMaterial);
	if (this->_currentSmoothing != -1)
		newLine.setSmoothing(this->_currentSmoothing);
	this->_data.addLine(newLine);
}

void FileParser::_setObject( std::stringstream& ss ) {
	std::string objectName;
	if (!(ss >> objectName))
		throw ParsingException("Invalid object");
	this->_currentObject = objectName;
}

void FileParser::_setGroup( std::stringstream& ss ) {
	std::string groupName;
	if (!(ss >> groupName))
		throw ParsingException("Invalid group");
	this->_currentGroup = groupName;
}

void FileParser::_setMaterial( std::stringstream& ss ) {
	std::string materialName;
	if (!(ss >> materialName))
		throw ParsingException("Invalid material");
	this->_currentMaterial = materialName;
}

void FileParser::_setSmoothing( std::stringstream& ss ) {
	std::string smoothing;
	if (!(ss >> smoothing))
		throw ParsingException("Invalid smoothing value");
	try {
		this->_currentSmoothing = std::stoul(smoothing);
	}
	catch (std::exception const& e) {
		throw ParsingException("Invalid smoothing value: " + smoothing);
	}
}

double FileParser::_parseDouble( std::string const& strNumber) const {
	try {
		return std::stold(strNumber);
	}
	catch (std::invalid_argument const& e) {
		throw ParsingException("Invalid number parsed: " + strNumber);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("Invalid number parsed: " + strNumber);
	}
}

int FileParser::_parseInt( std::string const& strNumber ) const {
	try {
		return std::stoi(strNumber);
	}
	catch (std::invalid_argument const& e) {
		throw ParsingException("Invalid number parsed: " + strNumber);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("Invalid number parsed: " + strNumber);
	}
}

void FileParser::parse( void ) {
	std::ifstream streamFile(this->_fileName);
	if (!streamFile)
		throw ParsingException("Error while opening file: " + this->_fileName);

	this->_currentObject = "";
	this->_currentGroup = "";
	this->_currentSmoothing = -1;
	this->_currentMaterial = "";
	try {
		std::string line, token;
		while (std::getline(streamFile, line)) {
			std::stringstream ss(line);
			ss >> token;
			if (token == "mtllip")
				this->_addFile(ss);
			else if (token == "v")
				this->_addVertex(ss);
			else if (token == "vt")
				this->_addTexture(ss);
			else if (token == "vn")
				this->_addVertexNorm(ss);
			else if (token == "vp")
				this->_addSpaceVertex(ss);
			else if (token == "f")
				this->_addFace(ss);
			else if (token == "l")
				this->_addLine(ss);
			else if (token == "o")
				this->_setObject(ss);
			else if (token == "g")
				this->_setGroup(ss);
			else if (token == "usemtl")
				this->_setMaterial(ss);
			else if (token == "s")
				this->_setSmoothing(ss);
			else if (token.substr(0) == "#")
				continue;
			else
				 throw ParsingException("Invalid directive []" + token + "] in line: " + line + ", parsing aborted");
		}
	}
	catch (ParsingException const& error) {
		streamFile.close();
		throw error;
	}
	streamFile.close();
	this->_dataParsed = true;
}

std::string const& FileParser::getFileName( void ) const noexcept {
	return this->_fileName;
}

ObjData const& FileParser::getData( void ) const {
	if (! this->_dataParsed)
		throw ParsingException("Data not parsed, call .parse() first");
	return this->_data;
}
