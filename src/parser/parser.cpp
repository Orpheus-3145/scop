#include "parser/parser.hpp"


FileParser::~FileParser( void ) noexcept {
	if (this->_data)
		delete this->_data;
}

void FileParser::parse( std::string const& fileName ) {
	this->_fileName = fileName;
	std::ifstream streamFile(this->_fileName);
	if (!streamFile)
		throw ParsingException("Error while opening file: " + this->_fileName);

	this->_data = new ObjData();
	this->_currentObject = "";
	this->_currentGroup = "";
	this->_currentSmoothing = -1;
	this->_currentMaterial = "";
	try {
		std::string line, lineType, lineContent;
		while (std::getline(streamFile, line)) {
			// skip empty lines
			if (line.length() == 0)
				continue;
			size_t spacePos = line.find(' ');
			if (spacePos == std::string::npos)
				throw ParsingException("Invalid line, no spaces: " + line);
			lineType = line.substr(0, spacePos);
			lineContent = line.substr(spacePos + 1);
			if (lineType == "mtllib")
				this->_addFile(lineContent);
			else if (lineType == "v")
				this->_addVertex(lineContent);
			else if (lineType == "vt")
				this->_addTexture(lineContent);
			else if (lineType == "vn")
				this->_addVertexNorm(lineContent);
			else if (lineType == "vp")
				this->_addSpaceVertex(lineContent);
			else if (lineType == "f")
				this->_addFace(lineContent);
			else if (lineType == "l")
				this->_addLine(lineContent);
			else if (lineType == "o")
				this->_setObject(lineContent);
			else if (lineType == "g")
				this->_setGroup(lineContent);
			else if (lineType == "usemtl")
				this->_setMaterial(lineContent);
			else if (lineType == "s")
				this->_setSmoothing(lineContent);
			// skip comments
			else if (lineType.substr(0, 1) == "#")
				continue;
			else
				 throw ParsingException("Invalid directive [" + lineType + "] in line: " + line + ", parsing aborted");
		}
	}
	catch (ParsingException const& error) {
		streamFile.close();
		throw error;
	}
	streamFile.close();
}

std::string const& FileParser::getFileName( void ) const noexcept {
	return this->_fileName;
}

ObjData const& FileParser::getData( void ) const {
	if (! this->_data)
		throw ParsingException("Data not parsed, call .parse() first");
	return *this->_data;
}

void FileParser::showData( void ) const {
	if (! this->_data)
		throw ParsingException("Data not parsed, call .parse() first");
	std::cout << *this->_data;
}

void FileParser::_addFile( std::string const& content ) {
	if (! this->_data)
		throw ParsingException("Data not parsed, call .parse() first");

	this->_data->addTmlFile(content);
}

void FileParser::_addVertex( std::string const& content ) {
	if (! this->_data)
		throw ParsingException("Data not parsed, call .parse() first");

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

	this->_data->addVertex(coorList);
}

void FileParser::_addTexture( std::string const& content ) {
	if (! this->_data)
		throw ParsingException("Data not parsed, call .parse() first");

	std::stringstream ss(content);
	std::vector<double> coorList;
	std::string coor;
	if (!(ss >> coor))
		throw ParsingException("Not enough texture coordinates provided: " + content);

	coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		coorList.push_back(this->_parseDouble(coor));
	if (ss >> coor)
		throw ParsingException("Too many texture coordinates provided: " + content);

	this->_data->addTextureCoor(coorList);
}

void FileParser::_addVertexNorm( std::string const& content ) {
	if (! this->_data)
		throw ParsingException("Data not parsed, call .parse() first");

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

	this->_data->addVertexNorm(coorList);
}

void FileParser::_addSpaceVertex( std::string const& content ) {
	if (! this->_data)
		throw ParsingException("Data not parsed, call .parse() first");

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

	this->_data->addParamSpaceVertex(coorList);
}

void FileParser::_addFace( std::string const& content ) {
	if (! this->_data)
		throw ParsingException("Data not parsed, call .parse() first");

	std::stringstream ss(content);
	std::vector<FaceCoor> indexList;
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
			coorList.push_back(this->_parseInt(strNumber));
		}
		indexList.push_back(FaceCoor(coorList));
	}
	if (indexList.size() < 3)
		throw ParsingException("Not enought face coordinates provided, minimum 3: " + content);

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
	this->_data->addFace(newFace);
}

void FileParser::_addLine( std::string const& content ) {
	if (! this->_data)
		throw ParsingException("Data not parsed, call .parse() first");

	std::stringstream ss(content);
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
	this->_data->addLine(newLine);
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
	if (content == "off") {
		this->_currentSmoothing = -1;
		return;
	}
	int smoothing = this->_parseInt(content);
	if (smoothing < 0)
		throw ParsingException("Invalid smoothing value, has to be non-negative: " + content);
	this->_currentSmoothing = smoothing;
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
