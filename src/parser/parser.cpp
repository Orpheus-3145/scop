#include "parser/parser.hpp"


std::shared_ptr<ParsedData> FileParser::parse( std::string const& fileName ) {

	std::shared_ptr<ParsedData> data = std::make_shared<ParsedData>();
	this->_currentObject = "";
	this->_currentGroup = "";
	this->_currentSmoothing = 0;
	this->_currentMaterial = "";
	this->_fileName = fileName;
	std::ifstream streamFile(this->_fileName);
	if (!streamFile)
		throw ParsingException("Error while opening file: " + this->_fileName);

	try {
		std::string line, lineType, lineContent;
		while (std::getline(streamFile, line)) {
			// skip empty lines
			if (line.length() == 0)
				continue;
			unsigned int leftTrim = 0;
			while (line[0] == ' ')
				leftTrim += 1;
			if (leftTrim > 0)
				line = line.substr(leftTrim);
			// skip comments
			if (line[0] == '#')
				continue;
			size_t spacePos = line.find(' ');
			if (spacePos == std::string::npos)
				throw ParsingException("Invalid line, no spaces: " + line);
			lineType = line.substr(0, spacePos);
			lineContent = line.substr(spacePos + 1);
			if (lineType == "mtllib")
				data->addTmlFile(this->_createFile(lineContent));
			else if (lineType == "v")
				data->addVertex(this->_createVertex(lineContent));
			else if (lineType == "vt")
				data->addTextureCoor(this->_createTexture(lineContent));
			else if (lineType == "vn")
				data->addVertexNorm(this->_createVertexNorm(lineContent));
			else if (lineType == "vp")
				data->addParamSpaceVertex(this->_createSpaceVertex(lineContent));
			else if (lineType == "f")
				data->addFace(this->_createFace(lineContent));
			else if (lineType == "l")
				data->addLine(this->_createLine(lineContent));
			else if (lineType == "o")
				this->_setObject(lineContent);
			else if (lineType == "g")
				this->_setGroup(lineContent);
			else if (lineType == "usemtl")
				this->_setMaterial(lineContent);
			else if (lineType == "s")
				this->_setSmoothing(lineContent);
			else
				throw ParsingException("Invalid directive [" + lineType + "] in line: " + line);
		}
	}
	catch (ParsingException const& error) {
		streamFile.close();
		throw error;
	}
	streamFile.close();
	// NB check if at least 3 vertexes are present
	// NB apply ear clipping / fan triangulation so every face has exactly 3 groups of indexes
	// this method only works if every face has exactly 3 coors
	return data;
}

std::string FileParser::_createFile( std::string const& content ) const {
	// NB do some checks? e.g. correct file extension, only one word, ... TBD
	return content;
}

VertexCoor FileParser::_createVertex( std::string const& content ) const {
	std::stringstream ss(content);
	std::vector<double> coorList;
	std::string coor;

	for (int i=0;i<3;i++) {
		if (!(ss >> coor))
			throw ParsingException("Not enough vertex coordinates provided: " + content);
		coorList.push_back(this->_parseDouble(coor));
	}
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		throw ParsingException("Too many vertex coordinates provided: " + content);

	return VertexCoor(coorList);
}

TextureCoor FileParser::_createTexture( std::string const& content ) const {
	std::stringstream ss(content);
	std::vector<double> coorList;
	std::string coor;
	if (!(ss >> coor))
		throw ParsingException("Not enough texture coordinates provided: " + content);
	// NB check that texture values vary between 0 and 1
	coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		throw ParsingException("Too many texture coordinates provided: " + content);

	return TextureCoor(coorList);
}

VertexNormCoor FileParser::_createVertexNorm( std::string const& content ) const {
	std::stringstream ss(content);
	std::vector<double> coorList;
	std::string coor;

	for (int i=0;i<3;i++) {
		if (!(ss >> coor))
			throw ParsingException("Not enough vertexNorm coordinates provided: " + content);
		coorList.push_back(this->_parseDouble(coor));
	}
	if (ss >> coor)
		throw ParsingException("Too many vertexNorm coordinates provided: " + content);

	return VertexNormCoor(coorList);
}

VertexSpaceParamCoor FileParser::_createSpaceVertex( std::string const& content ) const {
	std::stringstream ss(content);
	std::vector<double> coorList;
	std::string coor;
	if (!(ss >> coor))
		throw ParsingException("Not enough paramSpaceVertex coordinates provided: " + content);
	coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		throw ParsingException("Too many paramSpaceVertex coordinates provided: " + content);

	return VertexSpaceParamCoor(coorList);
}

Face FileParser::_createFace( std::string const& content ) const {
	std::stringstream ss(content);
	std::vector<t_index3D> indexList;
	std::string index;

	while (ss >> index) {
		std::vector<unsigned int> coorList;
		std::stringstream ssCoor(index);
		std::string strNumber;
		while(std::getline(ssCoor, strNumber, '/')) {
			// getline considers the string "" as a valid split
			// in case of two consecutive slashes
			if (strNumber == "")
				continue;
			coorList.push_back(this->_parseUint(strNumber));
		}
		indexList.push_back(t_index3D(coorList));
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
	if (this->_currentSmoothing != 0)
		newFace.setSmoothing(this->_currentSmoothing);
	return newFace;
}

Line FileParser::_createLine( std::string const& content ) const {
	std::stringstream ss(content);
	std::vector<unsigned int> indexList;
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

void FileParser::_setObject( std::string const& content ) {
	this->_currentObject = content;
}

void FileParser::_setGroup( std::string const& content ) {
	this->_currentGroup = content;
}

void FileParser::_setMaterial( std::string const& content ) {
	this->_currentMaterial = content;
}

void FileParser::_setSmoothing( std::string const& content ) {
	if (content != "off")
		this->_currentSmoothing = this->_parseUint(content);
	else
		this->_currentSmoothing = 0;
}

double FileParser::_parseDouble( std::string const& strNumber) const {
	try {
		return std::stold(strNumber);
	}
	catch (std::invalid_argument const& e) {
		throw ParsingException("Invalid number parsed: " + strNumber);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("Invalid number parsed, overflow: " + strNumber);
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
		throw ParsingException("Invalid number parsed, overflow: " + strNumber);
	}
}

unsigned int FileParser::_parseUint( std::string const& strNumber ) const {
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
