#include "parser.hpp"


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

std::vector<VectUI3D> const& Face::getIndexes( void ) const noexcept {
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


float const* VBO::getData( void ) const {
	return this->data.get();
}

uint32_t const* EBO::getData( void ) const {
	return this->data.get();
}


std::vector<std::string> const& ParsedData::getTmlFiles( void ) const noexcept {
	return this->_tmlFiles;
}

std::vector<VectF3D> const& ParsedData::getVertices( void ) const noexcept {
	return this->_vertexes;
}

std::vector<VectF2D> const& ParsedData::getTextures( void ) const noexcept {
	return this->_textures;
}

std::vector<VectF3D> const& ParsedData::getVerticesNorm( void ) const noexcept {
	return this->_vertexNorms;
}

std::vector<VectF3D> const& ParsedData::getParamSpaceVertices( void ) const noexcept {
	return this->_paramSpaceVertices;
}

std::vector<Face> const& ParsedData::getFaces( void ) const noexcept {
	return this->_faces;
}

std::vector<Line> const& ParsedData::getLines( void ) const noexcept {
	return this->_lines;
}

std::shared_ptr<VBO> const& ParsedData::getVBO( void ) const {
	if (!this->_VBOdata)
		throw AppException("VBO not initialized, call .fillBuffers()");
	return this->_VBOdata;
}

std::shared_ptr<EBO> const& ParsedData::getEBO( void ) const {
	if (!this->_EBOdata)
		throw AppException("EBO not initialized, call .fillBuffers()");
	return this->_EBOdata;
}

bool ParsedData::hasFaces( void ) const noexcept {
	return this->_faces.size() > 0;
}

struct VectorByteHash {
	uint32_t operator()(const std::vector<std::byte>& arr) const noexcept {
		uint32_t h = 0, magicNo = 0x9e3779b9;
		for (auto b : arr)
			h ^= std::to_integer<uint32_t>(b) + magicNo + (h << 6) + (h >> 2);
		return h;
	}
};

struct VectorByteEqual {
	bool operator()(const std::vector<std::byte>& a, const std::vector<std::byte>& b) const noexcept {
		return a == b;
	}
};

void ParsedData::fillBuffers( void ) {
	if (this->_faces.size() == 0) {
		this->_fillVBOnoFaces();
		return;
	}

	std::unordered_map<std::vector<std::byte>,uint32_t,VectorByteHash,VectorByteEqual> uniqueData;
	const uint32_t 			vertexSize = ParsedData::VBO_STRIDE / sizeof(float); // 11, see header
	const uint32_t 			bufferIncrement = 24;
	uint32_t				uniqueIndex = 0;
	float*					currentVertex = nullptr;
	std::vector<float>		vbo;
	std::vector<uint32_t>	ebo;

	uint32_t indexColor = 0;
	std::array<VectF3D, 3> colors{
		VectF3D{randomFloat(), randomFloat(), randomFloat()},
		VectF3D{randomFloat(), randomFloat(), randomFloat()},
		VectF3D{randomFloat(), randomFloat(), randomFloat()}
	};

	for (Face const& face : this->_faces) {
		std::vector<std::vector<VectUI3D>> vertexIndexes;
		if (face.getIndexes().size() > 3)
			vertexIndexes = this->_earClip(face.getIndexes());
		else
			vertexIndexes.push_back(face.getIndexes());

		for (std::vector<VectUI3D> const& triangleIndex : vertexIndexes) {
			for (VectUI3D const& vertexIndex : triangleIndex) {
				std::vector<std::byte> serializedVertex = this->_serializeVertex(vertexIndex, face.getFaceType());

				if (uniqueData.count(serializedVertex) == 0) {
					uniqueData[serializedVertex] = uniqueIndex++;
					if (uniqueIndex * vertexSize > vbo.size()) {
						vbo.resize((uniqueIndex + bufferIncrement) * vertexSize);
						currentVertex = vbo.data() + (uniqueIndex - 1) * vertexSize;
					}
					// insert vertex data
					std::byte* dataToinsert = serializedVertex.data();
					std::memcpy(currentVertex, dataToinsert, sizeof(VectF3D));
					currentVertex += sizeof(VectF3D) / sizeof(float);
					dataToinsert += sizeof(VectF3D);
					// insert RGB
					std::memcpy(currentVertex, &colors[indexColor++ % 3], sizeof(VectF3D));
					currentVertex += sizeof(VectF3D) / sizeof(float);
					// insert texture coordinates
					std::memcpy(currentVertex, dataToinsert, sizeof(VectF2D));
					currentVertex += sizeof(VectF2D) / sizeof(float);
					dataToinsert += sizeof(VectF2D);
					// insert normals
					std::memcpy(currentVertex, dataToinsert, sizeof(VectF3D));
					currentVertex += sizeof(VectF3D) / sizeof(float);
				}
				ebo.push_back(uniqueData[serializedVertex]);
			}
		}
	}
	this->_VBOdata = std::make_shared<VBO>();
	this->_VBOdata->size = uniqueIndex;
	this->_VBOdata->stride = ParsedData::VBO_STRIDE;
	this->_VBOdata->data = std::make_unique<float[]>(uniqueIndex * vertexSize);
	std::move(vbo.data(), vbo.data() + uniqueIndex * vertexSize, this->_VBOdata->data.get());
	
	this->_EBOdata = std::make_shared<EBO>();
	this->_EBOdata->size = ebo.size();
	this->_EBOdata->stride = ParsedData::EBO_STRIDE;
	this->_EBOdata->data = std::make_unique<uint32_t[]>(ebo.size());
	std::move(ebo.data(), ebo.data() + ebo.size(), this->_EBOdata->data.get());
}

std::vector<std::byte> ParsedData::_serializeVertex( VectUI3D const& faceIndex, FaceType type ) const noexcept {
	std::vector<std::byte> serializedVertex;
	// the RGB of the vertex is not stored, since is a random value, so reduce the size of the data to serialize
	serializedVertex.resize(ParsedData::VBO_STRIDE - sizeof(VectF3D));
	std::byte* container = serializedVertex.data();

	VectF3D const& vertex = this->_vertexes[faceIndex.i1];
	std::memcpy(container, &vertex, sizeof(vertex));
	container += sizeof(vertex);

	VectF2D texture;
	if (type == VERTEX_TEXT or type == VERTEX_TEXT_VNORM)
		texture = this->_textures[faceIndex.i2];
	else
		texture = VectF2D{0.5f, 0.5f};
	std::memcpy(container, &texture, sizeof(texture));
	container += sizeof(texture);

	VectF3D normal;
	if (type == VERTEX_VNORM or type == VERTEX_TEXT_VNORM)
		normal = this->_vertexNorms[faceIndex.i3];
	else
		normal = VectF3D{0.5f, 0.5f, 0.5f};
	std::memcpy(container, &normal, sizeof(normal));

	return serializedVertex;
}

void ParsedData::_fillVBOnoFaces( void ) {
	if (this->_vertexes.size() == 0)
		throw AppException("No vertexes found in file");

	std::shared_ptr<VBO> vbo = std::make_shared<VBO>();
	vbo->size = this->_vertexes.size();
	vbo->stride = 6 * sizeof(float); // 3 floats (vertex) + 3 floats (RGB)
	vbo->data = std::make_unique<float[]>(vbo->size * 6);

	uint32_t indexColor = 0;
	std::array<VectF3D, 3> colors{
		VectF3D{randomFloat(), randomFloat(), randomFloat()},
		VectF3D{randomFloat(), randomFloat(), randomFloat()},
		VectF3D{randomFloat(), randomFloat(), randomFloat()}
	};

	float* vboPtr = vbo->data.get();
	for (VectF3D& vertex : this->_vertexes) {
		std::memcpy(vboPtr, &vertex, sizeof(VectF3D));
		vboPtr += sizeof(VectF3D);
		std::memcpy(vboPtr, &colors[indexColor++ % 3], sizeof(VectF3D));
		vboPtr += sizeof(VectF3D);
	}
	this->_VBOdata = std::move(vbo);
}

std::vector<std::vector<VectUI3D>> ParsedData::_earClip( std::vector<VectUI3D> const& faceIndexes ) const noexcept {
	std::list<std::pair<VectUI3D,VectF2D>> vertexes = _create2Dvertexes(faceIndexes);
	std::vector<std::vector<VectUI3D>> splitTriangles;

	std::list<VectF2D> convexVertexes;
	std::list<VectF2D> earVertexes;
	std::list<VectF2D> reflexVertexes;
	for (auto curr = vertexes.begin(); curr != vertexes.end(); ++curr) {
		if (this->_isConvex(curr, vertexes) == true) {
			convexVertexes.push_back((*curr).second);
			if (this->_isEar(curr, vertexes) == true)
				earVertexes.push_back((*curr).second);
		} else
			reflexVertexes.push_back((*curr).second);
	}

	while (vertexes.size() > 3) {
		std::list<std::pair<VectUI3D,VectF2D>>::const_iterator curr = vertexes.begin();
		if (std::find(earVertexes.begin(), earVertexes.end(), (*curr).second) == earVertexes.end())
			continue;
		earVertexes.erase(earVertexes.begin());

		std::list<std::pair<VectUI3D,VectF2D>>::const_iterator prev = std::prev(curr);
		if (curr == vertexes.begin())
			prev = std::prev(vertexes.end());
		std::list<std::pair<VectUI3D,VectF2D>>::const_iterator post = std::next(curr);
		if (post == vertexes.end())
			post = vertexes.begin();

		// if v(i +- 1) is convex check if becomes an ear
		if (std::find(convexVertexes.begin(), convexVertexes.end(), (*prev).second) != convexVertexes.end()) {
			if (this->_isEar(prev, vertexes) == true)
				earVertexes.push_back((*prev).second);
		}
		if (std::find(convexVertexes.begin(), convexVertexes.end(), (*post).second) != convexVertexes.end()) {
			if (this->_isEar(post, vertexes) == true)
				earVertexes.push_back((*post).second);
		}
		// if v(i +- 1) is an ear check if it remains an ear
		if (std::find(earVertexes.begin(), earVertexes.end(), (*prev).second) != earVertexes.end()) {
			if (this->_isEar(prev, vertexes) == false)
				earVertexes.remove((*prev).second);
		}
		if (std::find(earVertexes.begin(), earVertexes.end(), (*post).second) != earVertexes.end()) {
			if (this->_isEar(prev, vertexes) == false)
				earVertexes.remove((*post).second);
		}
		// if v(i +- 1) is reflex check if it becomes convex or even an ear
		if (std::find(reflexVertexes.begin(), reflexVertexes.end(), (*prev).second) != reflexVertexes.end()) {
			if (this->_isConvex(prev, vertexes) == true) {
				reflexVertexes.remove((*prev).second);
				convexVertexes.push_back((*prev).second);
				if (this->_isEar(prev, vertexes) == true)
					earVertexes.push_back((*post).second);
			}
		}
		if (std::find(reflexVertexes.begin(), reflexVertexes.end(), (*post).second) != reflexVertexes.end()) {
			if (this->_isConvex(post, vertexes) == true) {
				reflexVertexes.remove((*post).second);
				convexVertexes.push_back((*post).second);
				if (this->_isEar(post, vertexes) == true)
					earVertexes.push_back((*post).second);
			}
		}

		splitTriangles.push_back(std::vector<VectUI3D>{(*prev).first, (*curr).first, (*post).first});
		vertexes.erase(curr);
	}
	std::vector<VectUI3D> lastTriangle;
	for (auto const& [index, _] : vertexes)
		lastTriangle.push_back(index);
	splitTriangles.push_back(lastTriangle);
	return (splitTriangles);
}

std::list<std::pair<VectUI3D,VectF2D>> ParsedData::_create2Dvertexes( std::vector<VectUI3D> const& indexList ) const noexcept {
	// Newell algorithm to find the normal
	float nx = 0.0f, ny = 0.0f, nz = 0.0f;
	for(uint32_t i=0; i < indexList.size(); i++) {
		VectF3D current = this->_vertexes[indexList[i].i1];
		VectF3D next = this->_vertexes[indexList[(i + 1) % indexList.size()].i1];
		nx += (current.y - next.y) * (current.z + next.z);
		ny += (current.z - next.z) * (current.x + next.x);
		nz += (current.x - next.x) * (current.y + next.y);
	}

	VectF3D normal = VectF3D{nx, ny, nz};
	normal = normal / getNorm(normal);

	VectF3D u, v;
	if (fabs(normal.x) > fabs(normal.y))
		u = VectF3D{normal.z * -1, 0.0f, normal.x};
	else
		u = VectF3D{0.0f, normal.z * -1, normal.y};
	u = u / getNorm(u);
	v = normal * u;

	VectF3D p0 = this->_vertexes[indexList[0].i1];
	std::list<std::pair<VectUI3D,VectF2D>> vertexes;
	for (auto const& faceIndex : indexList) {
		VectF3D piOriginp0 = this->_vertexes[faceIndex.i1] - p0;
		VectF2D projectedPi = VectF2D{piOriginp0 ^ u, piOriginp0 ^ v};
		vertexes.push_back(std::pair<VectUI3D,VectF2D>(faceIndex, projectedPi));
	}
	return vertexes;
}

bool ParsedData::_isConvex(std::list<std::pair<VectUI3D,VectF2D>>::const_iterator const& curr, std::list<std::pair<VectUI3D,VectF2D>> const& vertexes) const noexcept {
	std::list<std::pair<VectUI3D,VectF2D>>::const_iterator prev = std::prev(curr);
	if (curr == vertexes.begin())
		prev = std::prev(vertexes.cend());
	std::list<std::pair<VectUI3D,VectF2D>>::const_iterator post = std::next(curr);
	if (post == vertexes.end())
		post = vertexes.begin();

	return width((*prev).second, (*curr).second, (*post).second) < M_PI;
}

bool ParsedData::_isEar(std::list<std::pair<VectUI3D,VectF2D>>::const_iterator const& curr, std::list<std::pair<VectUI3D,VectF2D>> const& vertexes) const noexcept {
	std::list<std::pair<VectUI3D,VectF2D>>::const_iterator prev = std::prev(curr);
	if (curr == vertexes.begin())
		prev = std::prev(vertexes.cend());
	std::list<std::pair<VectUI3D,VectF2D>>::const_iterator post = std::next(curr);
	if (post == vertexes.end())
		post = vertexes.begin();

	for (auto check = std::next(post); check != std::prev(prev); ++check) {
		if (check == vertexes.end())
			check = vertexes.begin();
		if (triangleContainmentTest((*prev).second, (*curr).second, (*post).second, (*check).second) == false)
			return false;
	}
	return true;

}

std::array<std::byte,ParsedData::VBO_STRIDE> ParsedData::_serialize( VectUI3D const& index, FaceType faceType ) const noexcept {
	std::array<std::byte,ParsedData::VBO_STRIDE> serializedVertex;
	std::byte* container = serializedVertex.data();

	VectF3D const& vertex = this->_vertexes[index.i1];
	std::memcpy(container, &vertex, sizeof(vertex));
	container += sizeof(vertex);

	VectF2D texture;
	if (faceType == VERTEX_TEXT or faceType == VERTEX_TEXT_VNORM)
		texture = this->_textures[index.i2];
	else
		texture = VectF2D{0.5f, 0.5f};
	std::memcpy(container, &texture, sizeof(texture));
	container += sizeof(texture);

	VectF3D normal;
	if (faceType == VERTEX_VNORM or faceType == VERTEX_TEXT_VNORM)
		normal = this->_vertexNorms[index.i3];
	else
		normal = VectF3D{0.5f, 0.5f, 0.5f};
	std::memcpy(container, &normal, sizeof(normal));

	return serializedVertex;
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
	// NB check if at least 3 vertexes are present
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
		data._vertexNorms.push_back(this->_createVertexNorm(lineContent));
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

VectF3D FileParser::_createVertex( std::string const& content ) const {
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

	return VectF3D{x / w, y / w, z / w};
}

VectF2D FileParser::_createTexture( std::string const& content ) const {
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

	return VectF2D{u, v};
}

VectF3D FileParser::_createVertexNorm( std::string const& content ) const {
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

	return VectF3D{x, y, z};
}

VectF3D FileParser::_createSpaceVertex( std::string const& content ) const {
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

	return VectF3D{u, v, w};
}

Face FileParser::_createFace( std::string const& content ) const {
	std::stringstream ss(content);
	std::vector<VectUI3D> indexList;
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
				throw ParsingException("Face index value 0 in line: '" + content + "', it has to be at least 1");
			// face indexes start at 1, hence the -1
			coorList.push_back(toInsert - 1);
		}
		indexList.push_back(VectUI3D::from_vector(coorList));
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


std::ostream& operator<<(std::ostream& os, FaceType type) {
	os << faceToString(type);
	return os;
}

std::ostream& operator<<(std::ostream& os, Face const& obj) {
	switch (obj.getFaceType()) {
		case (VERTEX):
			for (VectUI3D const& faceIndex : obj.getIndexes()) {
				os << faceIndex.i1 << " ";
			}
			break;
		case (VERTEX_TEXT):
			for (VectUI3D const& faceIndex : obj.getIndexes()) {
				os << faceIndex.i1 << "/" << faceIndex.i2 << " ";
			}
			break;
		case (VERTEX_VNORM):
			for (VectUI3D const& faceIndex : obj.getIndexes()) {
				os << faceIndex.i1 << "//" << faceIndex.i2 << " ";
			}
			break;
		case (VERTEX_TEXT_VNORM):
			for (VectUI3D const& faceIndex : obj.getIndexes()) {
				os << faceIndex.i1 << "/" << faceIndex.i2 << "/" << faceIndex.i3 << " ";
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

std::ostream& operator<<( std::ostream& os, VBO const& data) {
	for (uint32_t i=0; i < data.size; i++) {
		for (uint32_t j=0; j < data.stride / sizeof(float); j++)
			os << data.getData()[i * data.stride / sizeof(float) + j] << " ";
		os << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, EBO const& data) {
	uint32_t const* indexes = data.getData();

	for (uint32_t i=0; i<data.size; i++)
		os << *(indexes + i) << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, ParsedData const& obj) {
	for (std::string const& fileName : obj.getTmlFiles())
		os << "file: " << fileName << std::endl;
	for (VectF3D const& vertex : obj.getVertices())
		os << "v: " << vertex << std::endl;
	for (VectF2D const& texture : obj.getTextures())
		os << "vt: " << texture << std::endl;
	for (VectF3D const& vertexNorm : obj.getVerticesNorm())
		os << "vn: " << vertexNorm << std::endl;
	for (VectF3D const& vertexParam : obj.getParamSpaceVertices())
		os << "vp: " << vertexParam << std::endl;
	for (Face const& face : obj.getFaces())
		os << "f: " << face << std::endl;
	for (Line const& line : obj.getLines())
		os << "l: " << line << std::endl;
	return os;
}
