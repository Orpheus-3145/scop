#include "parser/parserTypes.hpp"


coor2D coor2D::from_vector( std::vector<float> const& coor) noexcept {
	float x = 0.0f, y = 0.0f;
	if (coor.size() > 0)
		x = coor[0];
	if (coor.size() > 1)
		y = coor[1];
	return coor2D{x, y};
}

coor3D coor3D::from_vector( std::vector<float> const& coor) {
	float x = 0.0f, y = 0.0f, z = 0.0f;
	if (coor.size() > 0)
		x = coor[0];
	if (coor.size() > 1)
		y = coor[1];
	if (coor.size() > 2)
		z = coor[2];
	if (coor.size() > 3) {
		if (coor[3] == 0.0f)
			throw ParsingException("Omogeneous coordinate is 0");
		x /= coor[3];
		y /= coor[3];
		z /= coor[3];
	}
	return coor3D{x, y, z};
}

index3D index3D::from_vector( std::vector<uint32_t> const& positions) noexcept {
	uint32_t i1 = 0U, i2 = 0U, i3 = 0U;
	if (positions.size() > 0)
		i1 = positions[0];
	if (positions.size() > 1)
		i2 = positions[1];
	if (positions.size() > 2)
		i3 = positions[2];
	return index3D{i1, i2, i3};
}


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

std::vector<index3D> const& Face::getCoors( void ) const noexcept {
	return this->_coors;
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

std::vector<uint32_t> const& Line::getCoors( void ) const noexcept {
	return this->_coors;
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

std::vector<coor3D> const& ParsedData::getVertices( void ) const noexcept {
	return this->_vertices;
}

std::vector<coor2D> const& ParsedData::getTextures( void ) const noexcept {
	return this->_textures;
}

std::vector<coor3D> const& ParsedData::getVerticesNorm( void ) const noexcept {
	return this->_verticesNorm;
}

std::vector<coor3D> const& ParsedData::getParamSpaceVertices( void ) const noexcept {
	return this->_paramSpaceVertices;
}

std::vector<Face> const& ParsedData::getFaces( void ) const noexcept {
	return this->_faces;
}

std::vector<Line> const& ParsedData::getLines( void ) const noexcept {
	return this->_lines;
}

std::shared_ptr<VBO> const& ParsedData::getVBO( void ) const {
	return this->_VBOdata;
}

std::shared_ptr<EBO> const& ParsedData::getEBO( void ) const {
	return this->_EBOdata;
}

void ParsedData::addTmlFile( std::string const& newFile ) noexcept {
	this->_tmlFiles.push_back(newFile);
}

void ParsedData::addVertex( coor3D const& newVertex ) noexcept {
	this->_vertices.push_back(newVertex);
}

void ParsedData::addTexture( coor2D const& newTexture ) noexcept {
	this->_textures.push_back(newTexture);
}

void ParsedData::addVertexNorm( coor3D const& newVertexNorm ) noexcept {
	this->_verticesNorm.push_back(newVertexNorm);
}

void ParsedData::addParamSpaceVertex( coor3D const& newVertexParam ) noexcept {
	this->_paramSpaceVertices.push_back(newVertexParam);
}

void ParsedData::addFace( Face const& newFace ) {
	this->_faces.push_back(newFace);
}

void ParsedData::addLine( Line const& newLine ) noexcept {
	this->_lines.push_back(newLine);
}

// std::shared_ptr<VBO> ParsedData::createVBO( FaceType type ) const {
// 	if (this->_faces.count(type)== 0)
// 		throw AppException("No faces of type " + faceToString(type));
//
// 	std::shared_ptr<VBO> vbo = std::make_shared<VBO>();
// 	vbo->size = this->_faces.at(type).size() * 3;
// 	unsigned int posInsert = 0;
//
// 	switch (type) {
// 		case VERTEX:
// 			vbo->stride = 3;
// 			vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX)) {
// 				for (t_index3D const& vertexCoor : face.getCoors()) {
// 					unsigned int vertexIndex = vertexCoor._i1 - 1;
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].x();
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].y();
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].z();
// 				}
// 			}
// 			break;
// 		case VERTEX_TEXT:
// 			vbo->stride = 5;
// 			vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX_TEXT)) {
// 				for (t_index3D const& vertexTextCoor : face.getCoors()) {
// 					unsigned int vertexIndex = vertexTextCoor._i1 - 1;
// 					unsigned int textureIndex = vertexTextCoor._i2 - 1;
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].x();
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].y();
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].z();
// 					vbo->data[posInsert++] = this->_textures[textureIndex].x();
// 					vbo->data[posInsert++] = this->_textures[textureIndex].y();
// 				}
// 			}
// 			break;
// 		case VERTEX_VNORM:
// 			vbo->stride = 6;
// 			vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX_VNORM)) {
// 				for (t_index3D const& vertexNormCoor : face.getCoors()) {
// 					unsigned int vertexIndex = vertexNormCoor._i1 - 1;
// 					unsigned int normalIndex = vertexNormCoor._i2 - 1;
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].x();
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].y();
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].z();
// 					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].x();
// 					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].y();
// 					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].z();
// 				}
// 			}
// 			break;
// 		case VERTEX_TEXT_VNORM:
// 			vbo->stride = 8;
// 			vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX_TEXT_VNORM)) {
// 				for (t_index3D const& vertexTextNormCoor : face.getCoors()) {
// 					unsigned int vertexIndex = vertexTextNormCoor._i1 - 1;
// 					unsigned int textureIndex = vertexTextNormCoor._i2 - 1;
// 					unsigned int normalIndex = vertexTextNormCoor._i3 - 1;
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].x();
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].y();
// 					vbo->data[posInsert++] = this->_vertices[vertexIndex].z();
// 					vbo->data[posInsert++] = this->_textures[textureIndex].x();
// 					vbo->data[posInsert++] = this->_textures[textureIndex].y();
// 					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].x();
// 					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].y();
// 					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].z();
// 				}
// 			}
// 			break;
// 	}
// 	return vbo;
// }
//
// std::shared_ptr<VBO> ParsedData::createVBO( void ) const noexcept {
// 	std::shared_ptr<VBO> vbo = std::make_shared<VBO>();
// 	vbo->size = std::max({this->_vertices.size(), this->_textures.size(), this->_verticesNorm.size()});
// 	if (this->_textures.size() > 0) {
// 		if (this->_verticesNorm.size() > 0) {
// 			vbo->type = VERTEX_TEXT_VNORM;
// 			vbo->stride = 8;
// 		}
// 		else {
// 			vbo->type = VERTEX_TEXT;
// 			vbo->stride = 5;
// 		}
// 	} else if (this->_verticesNorm.size() > 0) {
// 		vbo->type = VERTEX_VNORM;
// 		vbo->stride = 6;
// 	}
// 	else {
// 		vbo->type = VERTEX;
// 		vbo->stride = 3;
// 	}
// 	vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride);
//
// 	unsigned int posInsert = 0;
// 	for (unsigned int i=0; i<vbo->size; i++) {
// 		switch (vbo->type) {
// 			case VERTEX:
// 				vbo->data[posInsert++] = this->_vertices[i].getVertex()._x;
// 				vbo->data[posInsert++] = this->_vertices[i].getVertex()._y;
// 				vbo->data[posInsert++] = this->_vertices[i].getVertex()._z;
// 				break;
// 			case VERTEX_TEXT:
// 				if (i < this->_vertices.size()) {
// 					vbo->data[posInsert++] = this->_vertices[i].getVertex()._x;
// 					vbo->data[posInsert++] = this->_vertices[i].getVertex()._y;
// 					vbo->data[posInsert++] = this->_vertices[i].getVertex()._z;
// 				} else {
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 				}
// 				if (i < this->_textures.size()) {
// 					vbo->data[posInsert++] = this->_textures[i].getTextures()._x;
// 					vbo->data[posInsert++] = this->_textures[i].getTextures()._y;
// 				} else {
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 				}
// 				break;
// 			case VERTEX_VNORM:
// 				if (i < this->_vertices.size()) {
// 					vbo->data[posInsert++] = this->_vertices[i].getVertex()._x;
// 					vbo->data[posInsert++] = this->_vertices[i].getVertex()._y;
// 					vbo->data[posInsert++] = this->_vertices[i].getVertex()._z;
// 				} else {
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 				}
// 				if (i < this->_verticesNorm.size()) {
// 					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._x;
// 					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._y;
// 					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._z;
// 				} else {
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 				}
// 				break;
// 			case VERTEX_TEXT_VNORM:
// 				if (i < this->_vertices.size()) {
// 					vbo->data[posInsert++] = this->_vertices[i].getVertex()._x;
// 					vbo->data[posInsert++] = this->_vertices[i].getVertex()._y;
// 					vbo->data[posInsert++] = this->_vertices[i].getVertex()._z;
// 				} else {
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 				}
// 				if (i < this->_textures.size()) {
// 					vbo->data[posInsert++] = this->_textures[i].getTextures()._x;
// 					vbo->data[posInsert++] = this->_textures[i].getTextures()._y;
// 				} else {
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 				}
// 				if (i < this->_verticesNorm.size()) {
// 					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._x;
// 					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._y;
// 					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._z;
// 				} else {
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 					vbo->data[posInsert++] = 0.0f;
// 				}
// 				break;
// 		}
// 	}
// 	return vbo;
// }
//
// std::shared_ptr<EBO> ParsedData::createEBO( FaceType type ) const {
// 	if (this->_faces.count(type)== 0)
// 		throw AppException("No faces of type " + faceToString(type));
//
// 	std::shared_ptr<EBO> ebo = std::make_shared<EBO>();
// 	ebo->size = this->_faces.at(type).size();
// 	ebo->type = type;
// 	unsigned int posInsert = 0;
//
// 	switch (type) {
// 		case VERTEX:
// 			ebo->stride = 3;
// 			ebo->data = std::make_unique<unsigned int[]>(ebo->size * ebo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX)) {
// 				for (t_index3D const& vertexIndex : face.getCoors())
// 					ebo->data[posInsert++] = vertexIndex._i1 - 1;
// 			}
// 			break;
// 		case VERTEX_TEXT:
// 			ebo->stride = 6;
// 			ebo->data = std::make_unique<unsigned int[]>(ebo->size * ebo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX_TEXT)) {
// 				for (t_index3D const& vertexTextIndex : face.getCoors()) {
// 					ebo->data[posInsert++] = vertexTextIndex._i1 - 1;
// 					ebo->data[posInsert++] = vertexTextIndex._i2 - 1;
// 				}
// 			}
// 			break;
// 		case VERTEX_VNORM:
// 			ebo->stride = 6;
// 			ebo->data = std::make_unique<unsigned int[]>(ebo->size * ebo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX_VNORM)) {
// 				for (t_index3D const& vertexNormIndex : face.getCoors()) {
// 					ebo->data[posInsert++] = vertexNormIndex._i1 - 1;
// 					ebo->data[posInsert++] = vertexNormIndex._i2 - 1;
// 				}
// 			}
// 			break;
// 		case VERTEX_TEXT_VNORM:
// 			ebo->stride = 9;
// 			ebo->data = std::make_unique<unsigned int[]>(ebo->size * ebo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX_TEXT_VNORM)) {
// 				for (t_index3D const& vertexTextNormIndex : face.getCoors()) {
// 					ebo->data[posInsert++] = vertexTextNormIndex._i1 - 1;
// 					ebo->data[posInsert++] = vertexTextNormIndex._i2 - 1;
// 					ebo->data[posInsert++] = vertexTextNormIndex._i3 - 1;
// 				}
// 			}
// 			break;
// 	}
// 	return ebo;
// }

struct ArrayByteHash {
	template <std::size_t N>
	std::size_t operator()(const std::array<std::byte, N>& arr) const noexcept {
		std::size_t h = 0;
		for (auto b : arr) {
			h ^= std::to_integer<std::size_t>(b) + 0x9e3779b9 + (h << 6) + (h >> 2);
		}
		return h;
	}
};

struct ArrayByteEqual {
	template <std::size_t N>
	bool operator()(const std::array<std::byte, N>& a, const std::array<std::byte, N>& b) const noexcept {
		return a == b;
	}
};

// std::shared_ptr<EBO> ParsedData::createEBO( FaceType type ) const {
// 	if (this->_faces.count(type)== 0)
// 		throw AppException("No faces of type " + faceToString(type));
//
// 	std::shared_ptr<EBO> ebo = std::make_shared<EBO>();
// 	ebo->size = this->_faces.at(type).size();
// 	ebo->type = type;
// 	uint32_t posInsert = 0;
//
// 	switch (type) {
// 		case VERTEX:
// 			ebo->stride = 3;
// 			ebo->data = std::make_unique<uint32_t[]>(ebo->size * ebo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX)) {
// 				for (index3D const& vertexIndex : face.getCoors())
// 					ebo->data[posInsert++] = vertexIndex.i1 - 1;
// 			}
// 			break;
// 		case VERTEX_TEXT:
// 			ebo->stride = 6;
// 			ebo->data = std::make_unique<uint32_t[]>(ebo->size * ebo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX_TEXT)) {
// 				for (index3D const& vertexTextIndex : face.getCoors()) {
// 					ebo->data[posInsert++] = vertexTextIndex.i1 - 1;
// 					ebo->data[posInsert++] = vertexTextIndex.i2 - 1;
// 				}
// 			}
// 			break;
// 		case VERTEX_VNORM:
// 			ebo->stride = 6;
// 			ebo->data = std::make_unique<uint32_t[]>(ebo->size * ebo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX_VNORM)) {
// 				for (index3D const& vertexNormIndex : face.getCoors()) {
// 					ebo->data[posInsert++] = vertexNormIndex.i1 - 1;
// 					ebo->data[posInsert++] = vertexNormIndex.i2 - 1;
// 				}
// 			}
// 			break;
// 		case VERTEX_TEXT_VNORM:
// 			ebo->stride = 9;
// 			ebo->data = std::make_unique<uint32_t[]>(ebo->size * ebo->stride);
// 			for (Face const& face : this->_faces.at(VERTEX_TEXT_VNORM)) {
// 				for (index3D const& vertexTextNormIndex : face.getCoors()) {
// 					ebo->data[posInsert++] = vertexTextNormIndex.i1 - 1;
// 					ebo->data[posInsert++] = vertexTextNormIndex.i2 - 1;
// 					ebo->data[posInsert++] = vertexTextNormIndex.i3 - 1;
// 				}
// 			}
// 			break;
// 	}
// 	return ebo;
// }

void ParsedData::createBuffers( void ) noexcept {
	// |f |f |f |f|f|f|f|f| f | f | f |
	// |vx|vy|vz|r|g|b|u|v|vnx|vny|vnz| * nFaces
	this->_VBOdata = std::make_shared<VBO>();
	this->_VBOdata->size = this->_faces.size() * 3;
	this->_VBOdata->stride = 11;
	this->_VBOdata->data = std::make_unique<float[]>(this->_VBOdata->size * this->_VBOdata->stride);

	this->_EBOdata = std::make_shared<EBO>();
	this->_EBOdata->size = this->_faces.size();
	this->_EBOdata->stride = 3;
	this->_EBOdata->data = std::make_unique<unsigned int[]>(this->_EBOdata->size * this->_EBOdata->stride);

	// unsigned int currentIndex = 0;
	constexpr unsigned int strideInBytes = 11 * sizeof(float);
	std::unordered_set<std::array<std::byte,strideInBytes>,ArrayByteHash,ArrayByteEqual> uniqueData;
	for (Face const& face : this->_faces) {
		std::array<std::byte,strideInBytes> currentElement;
		std::byte* container = currentElement.data();
		coor3D& vertex = this->_vertices[face.getCoors()[0].i1];
		std::memcpy(container, &vertex,sizeof(vertex));
	}
}


std::ostream& operator<<(std::ostream& os, coor2D const& coor) {
	os << coor.x << " " << coor.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, coor3D const& coor) {
	os << coor.x << " " << coor.y << " " << coor.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, index3D const& index) {
	os << index.i1 << " " << index.i2 << " " << index.i3;
	return os;
}

std::ostream& operator<<( std::ostream& os, FaceType type ) {
	os << faceToString(type);
	return os;
}

std::ostream& operator<<(std::ostream& os, Face const& obj) {
	switch (obj.getFaceType()) {
		case (VERTEX):
			for (index3D const& faceIndex : obj.getCoors()) {
				os << faceIndex.i1 << " ";
			}
			break;
		case (VERTEX_TEXT):
			for (index3D const& faceIndex : obj.getCoors()) {
				os << faceIndex.i1 << "/" << faceIndex.i2 << " ";
			}
			break;
		case (VERTEX_VNORM):
			for (index3D const& faceIndex : obj.getCoors()) {
				os << faceIndex.i1 << "//" << faceIndex.i2 << " ";
			}
			break;
		case (VERTEX_TEXT_VNORM):
			for (index3D const& faceIndex : obj.getCoors()) {
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
	for (uint32_t lineIndex : obj.getCoors()) {
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
	for (uint32_t i=0; i<data.size; i++) {
		for (uint32_t j=0; j < data.stride; j++)
			os << data.getData()[i * data.stride + j] << " ";
		os << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, EBO const& data) {
	uint32_t const* indexes = data.getData();

	for (uint32_t i=0; i<data.size; i++) {
		switch (data.type) {
			case VERTEX:
				os << indexes[i * data.stride];
				os << " " << indexes[i * data.stride + 1];
				os << " " << indexes[i * data.stride + 2] << std::endl;
				break;
			case VERTEX_TEXT:
				os << indexes[i * data.stride] << "/" << indexes[i * data.stride + 1];
				os << " " << indexes[i * data.stride + 2]  << "/" << indexes[i * data.stride + 3];
				os << " " << indexes[i * data.stride + 4] << "/" << indexes[i * data.stride + 5] << std::endl;
				break;
			case VERTEX_VNORM:
				os << indexes[i * data.stride] << "//" << indexes[i * data.stride + 1];
				os << " " << indexes[i * data.stride + 2]  << "//" << indexes[i * data.stride + 3];
				os << " " << indexes[i * data.stride + 4] << "//" << indexes[i * data.stride + 5] << std::endl;
				break;
			case VERTEX_TEXT_VNORM:
				os << indexes[i * data.stride] << "/" << indexes[i * data.stride + 1] << "/" << indexes[i * data.stride + 2];
				os << " " << indexes[i * data.stride + 3]  << "/" << indexes[i * data.stride + 4] << "/" << indexes[i * data.stride + 5];
				os << " " << indexes[i * data.stride + 6] << "/" << indexes[i * data.stride + 7] << "/" << indexes[i * data.stride + 8] << std::endl;
				break;
		}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const ParsedData& obj) {
	for (std::string const& fileName : obj.getTmlFiles())
		os << "file: " << fileName << std::endl;
	for (coor3D const& vertex : obj.getVertices())
		os << "v: " << vertex << std::endl;
	for (coor2D const& texture : obj.getTextures())
		os << "vt: " << texture << std::endl;
	for (coor3D const& vertexNorm : obj.getVerticesNorm())
		os << "vn: " << vertexNorm << std::endl;
	for (coor3D const& vertexParam : obj.getParamSpaceVertices())
		os << "vp: " << vertexParam << std::endl;
	for (Face const& face : obj.getFaces())
		os << "f: " << face << std::endl;
	for (Line const& line : obj.getLines())
		os << "l: " << line << std::endl;
	return os;
}
