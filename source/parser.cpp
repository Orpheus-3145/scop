#include <fstream>
#include <sstream>

#include "parser.hpp"
#include "data.hpp"
#include "exception.hpp"


std::string	faceToString( FaceType face ) {
	switch (face) {
		case VERTEX:
			return "VERTEX";
		case VERTEX_TEXT:
			return "VERTEX_TEXT";
		case VERTEX_VNORM:
			return "VERTEX_VNORM";
		case VERTEX_TEXT_VNORM:
			return "VERTEX_TEXT_VNORM";
		default:
			return "[invalid value: " + std::to_string(face) + "]";
	}
}


void Face::setFaceType( FaceType newType ) noexcept {
	this->_type = newType;
}

void Face::setIndexes( std::vector<VectUI3> const& indexes ) noexcept {
	this->_indexes = indexes;
}

void Face::setObject( std::string const& newObject ) noexcept {
	this->_object = newObject;
}

void Face::setGroup( std::string const& newGroup ) noexcept {
	this->_group = newGroup;
}

void Face::setMaterial( std::string const& newMaterial) noexcept {
	this->_material = newMaterial;
}

void Face::setSmoothing( uint32_t newSmoothing ) noexcept {
	this->_smoothing = newSmoothing;
}

FaceType Face::getFaceType( void ) const noexcept {
	return this->_type;
}

std::vector<VectUI3> const& Face::getIndexes( void ) const noexcept {
	return this->_indexes;
}

std::string Face::getObject( void ) const noexcept {
	return this->_object;
}

std::string Face::getGroup( void ) const noexcept {
	return this->_group;
}

std::string Face::getMaterial( void ) const noexcept {
	return this->_material;
}

uint32_t Face::getSmoothing( void ) const noexcept {
	return this->_smoothing;
}


void Line::setObject( std::string const& newObject ) noexcept {
	this->_object = newObject;
}

void Line::setGroup( std::string const& newGroup ) noexcept {
	this->_group = newGroup;
}

void Line::setMaterial( std::string const& newMaterial) noexcept {
	this->_material = newMaterial;
}

void Line::setSmoothing( uint32_t newSmoothing ) noexcept {
	this->_smoothing = newSmoothing;
}

std::vector<uint32_t> const& Line::getIndexes( void ) const noexcept {
	return this->_indexes;
}

std::string Line::getObject( void ) const noexcept {
	return this->_object;
}

std::string Line::getGroup( void ) const noexcept {
	return this->_group;
}

std::string Line::getMaterial( void ) const noexcept {
	return this->_material;
}

uint32_t Line::getSmoothing( void ) const noexcept {
	return this->_smoothing;
}


std::ostream& operator<<(std::ostream& os, FaceType type) {
	os << faceToString(type);
	return os;
}

std::ostream& operator<<(std::ostream& os, Face const& obj) {
	switch (obj.getFaceType()) {
		case (VERTEX):
			for (VectUI3 const& faceIndex : obj.getIndexes()) {
				os << faceIndex.i1 + 1 << " ";
			}
			break;
		case (VERTEX_TEXT):
			for (VectUI3 const& faceIndex : obj.getIndexes()) {
				os << faceIndex.i1 + 1 << "/" << faceIndex.i2 + 1 << " ";
			}
			break;
		case (VERTEX_VNORM):
			for (VectUI3 const& faceIndex : obj.getIndexes()) {
				os << faceIndex.i1 + 1 << "//" << faceIndex.i3 + 1 << " ";
			}
			break;
		case (VERTEX_TEXT_VNORM):
			for (VectUI3 const& faceIndex : obj.getIndexes()) {
				os << faceIndex.i1 + 1 << "/" << faceIndex.i2 + 1 << "/" << faceIndex.i3 + 1 << " ";
			}
			break;
	}
	if (obj.getObject().length() > 0)
		os << "|object: " << obj.getObject() << "|";
	if (obj.getGroup().length() > 0)
		os << "|group: " << obj.getGroup() << "|";
	if (obj.getMaterial().length() > 0)
		os << "|material: " << obj.getMaterial() << "|";
	if (obj.getSmoothing() != 0)
		os << "|smoothing: " << obj.getSmoothing() << "|";
	return os;
}

std::ostream& operator<<(std::ostream& os, Line const& obj) {
	for (uint32_t lineIndex : obj.getIndexes()) {
		os << lineIndex << " ";
	}
	if (obj.getObject().length() > 0)
		os << "|object: " << obj.getObject() << "|";
	if (obj.getGroup().length() > 0)
		os << "|group: " << obj.getGroup() << "|";
	if (obj.getMaterial().length() > 0)
		os << "|material: " << obj.getMaterial() << "|";
	if (obj.getSmoothing() != 0)
		os << "|smoothing: " << obj.getSmoothing() << "|";
	return os;
}


ParsedData FileParser::parse( std::string const& fileName ) {
	ParsedData data;
	this->_currentObject = "";
	this->_currentGroup = "";
	this->_currentSmoothing = 0;
	this->_currentMaterial = "";

	std::ifstream streamFile(fileName);
	if (!streamFile)
		throw ParsingException("Error while opening file: " + fileName);

	try {
		std::string line;
		while (std::getline(streamFile, line)) {
			line = this->_trimString(line);
			// skip empty lines
			if (line.length() == 0)
			continue;
			// skip comments
			if (line[0] == '#')
			continue;
			this->_parseDirective(line, data);
		}
	}
	catch (ParsingException const& error) {
		streamFile.close();
		throw error;
	}
	streamFile.close();
	return data;
}

void FileParser::_parseDirective( std::string const& line, ParsedData& data ) {
	size_t spacePos = line.find(' ');
	if (spacePos == std::string::npos)
		throw ParsingException("Invalid line: '" + line + "'");

	std::string lineType = this->_trimString(line.substr(0, spacePos));
	std::string lineContent = this->_trimString(line.substr(spacePos + 1));

	if (lineType == "mtllib")
		data._tmlFiles.push_back(this->_createFile(lineContent));
	else if (lineType == "v")
		data._vertexes.push_back(this->_createVertex(lineContent));
	else if (lineType == "vt")
		data._textures.push_back(this->_createTexture(lineContent));
	else if (lineType == "vn")
		data._normals.push_back(this->_createVertexNorm(lineContent));
	else if (lineType == "vp")
		data._paramSpaceVertices.push_back(this->_createSpaceVertex(lineContent));
	else if (lineType == "f")
		data._faces.push_back(this->_createFace(lineContent));
	else if (lineType == "l")
		data._lines.push_back(this->_createLine(lineContent));
	else if (lineType == "o")
		this->_currentObject = lineContent;
	else if (lineType == "g")
		this->_currentGroup = lineContent;
	else if (lineType == "usemtl")
		this->_currentMaterial = lineContent;
	else if (lineType == "s") {
		if (lineContent != "off")
			this->_currentSmoothing = this->_parseUint(lineContent);
		else
			this->_currentSmoothing = 0U;
	}
	else
		throw ParsingException("Invalid directive ['" + lineType + "'] in line: " + line);
}

std::string FileParser::_createFile( std::string const& content ) const {
	// NB do some checks? e.g. correct file extension, only one word, ... TBD
	return content;
}

VectF3 FileParser::_createVertex( std::string const& content ) const {
	std::stringstream ss(content);
	std::string coor;
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;

	if (!(ss >> coor))
		throw ParsingException("Not enough vertex coordinates provided: " + content);
	x = this->_parseFloat(coor);

	if (!(ss >> coor))
		throw ParsingException("Not enough vertex coordinates provided: " + content);
	y = this->_parseFloat(coor);

	if (!(ss >> coor))
		throw ParsingException("Not enough vertex coordinates provided: " + content);
	z = this->_parseFloat(coor);

	if (ss >> coor) {
		w = this->_parseFloat(coor);
		if (w == 0.0f)
			throw ParsingException("Homogeneus coordinate has value 0 (zero division error)");
	}

	if (ss >> coor)
		throw ParsingException("Too many vertex coordinates provided: " + content);

	return VectF3{x / w, y / w, z / w};
}

VectF2 FileParser::_createTexture( std::string const& content ) const {
	std::stringstream ss(content);
	std::string coor;
	float u = 0.0f, v = 0.0f;

	if (!(ss >> coor))
		throw ParsingException("Not enough texture coordinates provided: " + content);
	u = this->_parseFloat(coor);

	if (ss >> coor)
		v = this->_parseFloat(coor);

	if (ss >> coor)
		throw ParsingException("3D textures are not supported: " + content);

	return VectF2{u, v};
}

VectF3 FileParser::_createVertexNorm( std::string const& content ) const {
	std::stringstream ss(content);
	std::string coor;
	float x = 0.0f, y = 0.0f, z = 0.0f;

	if (!(ss >> coor))
		throw ParsingException("Not enough normal coordinates provided: " + content);
	x = this->_parseFloat(coor);

	if (!(ss >> coor))
		throw ParsingException("Not enough normal coordinates provided: " + content);
	y = this->_parseFloat(coor);

	if (!(ss >> coor))
		throw ParsingException("Not enough normal coordinates provided: " + content);
	z = this->_parseFloat(coor);

	if (ss >> coor)
		throw ParsingException("Too many normal coordinates provided: " + content);

	return VectF3{x, y, z};
}

VectF3 FileParser::_createSpaceVertex( std::string const& content ) const {
	std::stringstream ss(content);
	std::string coor;
	float u = 0.0f, v = 0.0f, w = 0.0f;

	if (!(ss >> coor))
		throw ParsingException("Not enough paramSpaceVertex coordinates provided: " + content);
	u = this->_parseFloat(coor);

	if (ss >> coor)
		v = this->_parseFloat(coor);

	if (ss >> coor)
		w = this->_parseFloat(coor);

	if (ss >> coor)
		throw ParsingException("Too many paramSpaceVertex coordinates provided: " + content);

	return VectF3{u, v, w};
}

Face FileParser::_createFace( std::string const& content ) const {
	std::stringstream ss(content);
	std::vector<VectUI3> indexList;
	std::string index;

	// split group of indexes (e.g. 1 or 1/2 or 1/4/5 or 1//3)
	while (ss >> index) {
		std::vector<uint32_t> coorList;
		std::stringstream ssCoor(index);
		std::string strNumber;
		// split group into vertex index [, texture index, normal index]
		while(std::getline(ssCoor, strNumber, '/')) {
			// getline considers the string "" as a valid split
			// in case of two consecutive slashes
			if (strNumber == "")
				continue;
			uint32_t toInsert = this->_parseUint(strNumber);
			if (toInsert == 0UL)
				throw ParsingException("Face index value 0 in line: '" + content + "', has to be at least 1");
			// face indexes start at 1, hence the -1
			coorList.push_back(toInsert - 1);
		}
		indexList.push_back(VectUI3::from_vector(coorList));
	}
	if (indexList.size() < 3)
		throw ParsingException("Not enought face coordinates provided, minimum 3: " + content);

	// NB protect from this: f 1/10 2/12/122 3/13/123
	FaceType _type;
	size_t firstSlashPos, secondSlashPos;
	firstSlashPos = index.find("/");
	if (firstSlashPos == std::string::npos)
		_type = VERTEX;
	else {
		secondSlashPos = index.find("/", firstSlashPos + 1);
		if (secondSlashPos == firstSlashPos + 1) {
			_type = VERTEX_VNORM;
			// swap the two indexes so that the norm index is always the third in the index struct
			for (VectUI3& coor : indexList)
				std::swap(coor.i2, coor.i3);
		}
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
	if (this->_currentSmoothing != 0)
		newFace.setSmoothing(this->_currentSmoothing);
	return newFace;
}

Line FileParser::_createLine( std::string const& content ) const {
	std::stringstream ss(content);
	std::vector<uint32_t> indexList;
	std::string index;

	while (ss >> index)
		indexList.push_back(this->_parseUint(index));

	Line newLine(indexList);
	if (this->_currentObject != "")
		newLine.setObject(this->_currentObject);
	if (this->_currentGroup != "")
		newLine.setGroup(this->_currentGroup);
	if (this->_currentMaterial != "")
		newLine.setMaterial(this->_currentMaterial);
	if (this->_currentSmoothing != 0)
		newLine.setSmoothing(this->_currentSmoothing);
	return newLine;
}

std::string FileParser::_trimString( std::string const& content ) const {
	size_t spacePos = content.find(' ');
	if (spacePos == std::string::npos)
		return content;
	else if (content == " ")
		return "";

	bool trimmed = false;
	uint32_t leftTrim = 0;
	uint32_t rightTrim = content.size() - 1;
	while (content[leftTrim] == ' ' or content[rightTrim] == ' ') {
		trimmed = true;
		if (content[leftTrim] == ' ')
			leftTrim++;
		if (content[rightTrim] == ' ')
			rightTrim--;
		if (leftTrim <= rightTrim)
			break;
	}
	if (trimmed)
		return content.substr(leftTrim, rightTrim + 1);
	else
		return content;
}

float FileParser::_parseFloat( std::string const& strNumber ) const {
	try {
		return std::stof(strNumber);
	}
	catch (std::invalid_argument const& e) {
		throw ParsingException("Invalid number parsed: " + strNumber);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("Invalid number parsed, overflow: " + strNumber);
	}
}

int32_t FileParser::_parseInt( std::string const& strNumber ) const {
	try {
		return std::stoi(strNumber);
	}
	catch (std::invalid_argument const& e) {
		throw ParsingException("Invalid number parsed: " + strNumber);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("Invalid number parsed, overflow: " + strNumber);
	}
}

uint32_t FileParser::_parseUint( std::string const& strNumber ) const {
	try {
		if (strNumber.size() > 0 and strNumber[0] == '-')
			throw ParsingException("Negative number parsed: " + strNumber);
		return std::stoul(strNumber);
	}
	catch (std::invalid_argument const& e) {
		throw ParsingException("Invalid number parsed: " + strNumber);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("Invalid number parsed, overflow: " + strNumber);
	}
}
