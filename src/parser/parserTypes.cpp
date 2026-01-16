#include "parser/parserTypes.hpp"


coor3D::coor3D( coor3D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	this->_z = coor._z;
}

coor3D::coor3D( std::vector<float> const& coors) {
	if (coors.size() > 0)
		this->_x = coors[0];
	if (coors.size() > 1)
		this->_y = coors[1];
	if (coors.size() > 2)
		this->_z = coors[2];
	if (coors.size() > 3)
		this->_y = coors[3];
}

coor3D& coor3D::operator=(coor3D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	this->_z = coor._z;
	return *this;
}


index3D::index3D( index3D const& coor) noexcept {
	this->_i1 = coor._i1;
	this->_i2 = coor._i2;
	this->_i3 = coor._i3;
}

index3D::index3D( std::vector<unsigned int> const& positions ) noexcept : index3D() {
	if (positions.size() > 0)
		this->_i1 = positions[0];
	if (positions.size() > 1)
		this->_i2 = positions[1];
	if (positions.size() > 2)
		this->_i3 = positions[2];
}

index3D& index3D::operator=(index3D const& coor) noexcept {
	this->_i1 = coor._i1;
	this->_i2 = coor._i2;
	this->_i3 = coor._i3;
	return *this;
}


VertexCoor::VertexCoor( std::vector<float> const& coorVect ) {
	switch(coorVect.size()) {
		case 3:
			this->_coor = t_coor3D(coorVect[0], coorVect[1], coorVect[2]);
			break;
		case 4:
			this->_coor = t_coor3D(coorVect[0] / coorVect[3], coorVect[1] / coorVect[3], coorVect[2] / coorVect[3]);
			break;
		default:
			throw ParsingException("Invalid size for vector to initialize a VertexCoor instance, has to be 3 or 4");
	}
}

t_coor3D const& VertexCoor::getVertex( void ) const noexcept {
	return this->_coor;
}

float	VertexCoor::x( void ) const noexcept {
	return this->_coor._x;
}

float	VertexCoor::y( void ) const noexcept {
	return this->_coor._y;
}

float	VertexCoor::z( void ) const noexcept {
	return this->_coor._z;
}


TextureCoor::TextureCoor( std::vector<float> const& coorVect ) {
	switch(coorVect.size()) {
		case 1:
			this->_coor = t_coor3D(coorVect[0], 0.f, 0.f);
			break;
		case 2:
			this->_coor = t_coor3D(coorVect[0], coorVect[1], 0.f);
			break;
		case 3:
			this->_coor = t_coor3D(coorVect[0], coorVect[1], coorVect[2]);
			break;
		default:
			throw ParsingException("Invalid size for vector to initialize a TextureCoor instance, has to be greater at least 1");
	}
}

t_coor3D const& TextureCoor::getTexture( void ) const noexcept {
	return this->_coor;
}

float	TextureCoor::x( void ) const noexcept {
	return this->_coor._x;
}

float	TextureCoor::y( void ) const noexcept {
	return this->_coor._y;
}

float	TextureCoor::z( void ) const noexcept {
	return this->_coor._z;
}


VertexNormCoor::VertexNormCoor( std::vector<float> const& coorVect ) {
	if (coorVect.size() != 3)
		throw ParsingException("Invalid size for vector to initialize a VertexNorm instance, has to be 3");

	this->_coor = t_coor3D(coorVect[0], coorVect[1], coorVect[2]);
}

t_coor3D const& VertexNormCoor::getVertexNorm( void ) const noexcept {
	return this->_coor;
}

float	VertexNormCoor::x( void ) const noexcept {
	return this->_coor._x;
}

float	VertexNormCoor::y( void ) const noexcept {
	return this->_coor._y;
}

float	VertexNormCoor::z( void ) const noexcept {
	return this->_coor._z;
}


VertexSpaceParamCoor::VertexSpaceParamCoor( std::vector<float> const& coorVect ) {
	switch(coorVect.size()) {
		case 1:
			this->_coor = t_coor3D(coorVect[0], 0.f, 0.f);
			break;
		case 2:
			this->_coor = t_coor3D(coorVect[0], coorVect[1], 0.f);
			break;
		case 3:
			this->_coor = t_coor3D(coorVect[0], coorVect[1], coorVect[2]);
			break;
		default:
			throw ParsingException("Invalid size for vector to initialize a VertexSpaceParamCoor instance, has to be 1, 2 or 3");
	}
}

t_coor3D const& VertexSpaceParamCoor::getVertexSpaceParam( void ) const noexcept {
	return this->_coor;
}

float	VertexSpaceParamCoor::x( void ) const noexcept {
	return this->_coor._x;
}

float	VertexSpaceParamCoor::y( void ) const noexcept {
	return this->_coor._y;
}

float	VertexSpaceParamCoor::z( void ) const noexcept {
	return this->_coor._z;
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

void Face::setSmoothing( unsigned int newSmoothing ) noexcept {
	this->_smoothing = newSmoothing;
}

FaceType Face::getFaceType( void ) const noexcept {
	return this->_type;
}

std::vector<t_index3D> const& Face::getCoors( void ) const noexcept {
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

unsigned int Face::getSmoothing( void ) const noexcept {
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

void Line::setSmoothing( unsigned int newSmoothing ) noexcept {
	this->_smoothing = newSmoothing;
}

std::vector<unsigned int> const& Line::getCoors( void ) const noexcept {
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

unsigned int Line::getSmoothing( void ) const noexcept {
	return this->_smoothing;
}


float const* VBO::getData( void ) const {
	return this->data.get();
}

FaceType VBO::getType( void ) const {
	switch (this->stride) {
		case 3:
			return VERTEX;
		case 5:
			return VERTEX_TEXT;
		case 6:
			return VERTEX_VNORM;
		case 8:
			return VERTEX_TEXT_VNORM;
		default:
			throw AppException("Invalid stride value: " + std::to_string(this->stride));
	}
}


unsigned int const* EBO::getData( void ) const {
	return this->data.get();
}


std::vector<std::string> const& ParsedData::getTmlFiles( void ) const noexcept {
	return this->_tmlFiles;
}

std::vector<VertexCoor> const& ParsedData::getVertices( void ) const noexcept {
	return this->_vertices;
}

std::vector<TextureCoor> const& ParsedData::getTextureCoors( void ) const noexcept {
	return this->_textureCoors;
}

std::vector<VertexNormCoor> const& ParsedData::getVerticesNorm( void ) const noexcept {
	return this->_verticesNorm;
}

std::vector<VertexSpaceParamCoor> const& ParsedData::getParamSpaceVertices( void ) const noexcept {
	return this->_paramSpaceVertices;
}

std::map<FaceType, std::vector<Face>> const& ParsedData::getFaces( void ) const noexcept {
	return this->_faces;
}

std::vector<Line> const& ParsedData::getLines( void ) const noexcept {
	return this->_lines;
}

void ParsedData::addTmlFile( std::string const& newFile ) noexcept {
	this->_tmlFiles.push_back(newFile);
}

void ParsedData::addVertex( VertexCoor const& newVertex ) {
	this->_vertices.push_back(newVertex);
}

void ParsedData::addTextureCoor( TextureCoor const& newTexture ) {
	this->_textureCoors.push_back(newTexture);
}

void ParsedData::addVertexNorm( VertexNormCoor const& newVertexNorm ) {
	this->_verticesNorm.push_back(newVertexNorm);
}

void ParsedData::addParamSpaceVertex( VertexSpaceParamCoor const& newVertexParam ) {
	this->_paramSpaceVertices.push_back(newVertexParam);
}

void ParsedData::addFace( Face const& newFace ) noexcept {
	this->_faces[newFace.getFaceType()].push_back(newFace);
}

void ParsedData::addLine( Line const& newLine ) noexcept {
	this->_lines.push_back(newLine);
}

std::shared_ptr<VBO> ParsedData::createVBO( FaceType type ) const {
	if (this->_faces.count(type)== 0)
		throw AppException("No faces of type " + faceToString(type));

	std::shared_ptr<VBO> vbo = std::make_shared<VBO>();
	vbo->size = this->_faces.at(type).size() * 3;
	unsigned int posInsert = 0;

	switch (type) {
		case VERTEX:
			vbo->stride = 3;
			vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride);
			for (Face const& face : this->_faces.at(VERTEX)) {
				for (t_index3D const& vertexCoor : face.getCoors()) {
					unsigned int vertexIndex = vertexCoor._i1 - 1;
					vbo->data[posInsert++] = this->_vertices[vertexIndex].x();
					vbo->data[posInsert++] = this->_vertices[vertexIndex].y();
					vbo->data[posInsert++] = this->_vertices[vertexIndex].z();
				}
			}
			break;
		case VERTEX_TEXT:
			vbo->stride = 5;
			vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride);
			for (Face const& face : this->_faces.at(VERTEX_TEXT)) {
				for (t_index3D const& vertexTextCoor : face.getCoors()) {
					unsigned int vertexIndex = vertexTextCoor._i1 - 1;
					unsigned int textureIndex = vertexTextCoor._i2 - 1;
					vbo->data[posInsert++] = this->_vertices[vertexIndex].x();
					vbo->data[posInsert++] = this->_vertices[vertexIndex].y();
					vbo->data[posInsert++] = this->_vertices[vertexIndex].z();
					vbo->data[posInsert++] = this->_textureCoors[textureIndex].x();
					vbo->data[posInsert++] = this->_textureCoors[textureIndex].y();
				}
			}
			break;
		case VERTEX_VNORM:
			vbo->stride = 6;
			vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride);
			for (Face const& face : this->_faces.at(VERTEX_VNORM)) {
				for (t_index3D const& vertexNormCoor : face.getCoors()) {
					unsigned int vertexIndex = vertexNormCoor._i1 - 1;
					unsigned int normalIndex = vertexNormCoor._i2 - 1;
					vbo->data[posInsert++] = this->_vertices[vertexIndex].x();
					vbo->data[posInsert++] = this->_vertices[vertexIndex].y();
					vbo->data[posInsert++] = this->_vertices[vertexIndex].z();
					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].x();
					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].y();
					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].z();
				}
			}
			break;
		case VERTEX_TEXT_VNORM:
			vbo->stride = 8;
			vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride);
			for (Face const& face : this->_faces.at(VERTEX_TEXT_VNORM)) {
				for (t_index3D const& vertexTextNormCoor : face.getCoors()) {
					unsigned int vertexIndex = vertexTextNormCoor._i1 - 1;
					unsigned int textureIndex = vertexTextNormCoor._i2 - 1;
					unsigned int normalIndex = vertexTextNormCoor._i3 - 1;
					vbo->data[posInsert++] = this->_vertices[vertexIndex].x();
					vbo->data[posInsert++] = this->_vertices[vertexIndex].y();
					vbo->data[posInsert++] = this->_vertices[vertexIndex].z();
					vbo->data[posInsert++] = this->_textureCoors[textureIndex].x();
					vbo->data[posInsert++] = this->_textureCoors[textureIndex].y();
					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].x();
					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].y();
					vbo->data[posInsert++] = this->_verticesNorm[normalIndex].z();
				}
			}
			break;
	}
	return vbo;
}

std::shared_ptr<VBO> ParsedData::createVBO( void ) const noexcept {
	std::shared_ptr<VBO> vbo = std::make_shared<VBO>();
	vbo->size = std::max({this->_vertices.size(), this->_textureCoors.size(), this->_verticesNorm.size()});
	if (this->_textureCoors.size() > 0) {
		if (this->_verticesNorm.size() > 0) {
			vbo->type = VERTEX_TEXT_VNORM;
			vbo->stride = 8;
		}
		else {
			vbo->type = VERTEX_TEXT;
			vbo->stride = 5;
		}
	} else if (this->_verticesNorm.size() > 0) {
		vbo->type = VERTEX_VNORM;
		vbo->stride = 6;
	}
	else {
		vbo->type = VERTEX;
		vbo->stride = 3;
	}
	vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride);

	unsigned int posInsert = 0;
	for (unsigned int i=0; i<vbo->size; i++) {
		switch (vbo->type) {
			case VERTEX:
				vbo->data[posInsert++] = this->_vertices[i].getVertex()._x;
				vbo->data[posInsert++] = this->_vertices[i].getVertex()._y;
				vbo->data[posInsert++] = this->_vertices[i].getVertex()._z;
				break;
			case VERTEX_TEXT:
				if (i < this->_vertices.size()) {
					vbo->data[posInsert++] = this->_vertices[i].getVertex()._x;
					vbo->data[posInsert++] = this->_vertices[i].getVertex()._y;
					vbo->data[posInsert++] = this->_vertices[i].getVertex()._z;
				} else {
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
				}
				if (i < this->_textureCoors.size()) {
					vbo->data[posInsert++] = this->_textureCoors[i].getTexture()._x;
					vbo->data[posInsert++] = this->_textureCoors[i].getTexture()._y;
				} else {
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
				}
				break;
			case VERTEX_VNORM:
				if (i < this->_vertices.size()) {
					vbo->data[posInsert++] = this->_vertices[i].getVertex()._x;
					vbo->data[posInsert++] = this->_vertices[i].getVertex()._y;
					vbo->data[posInsert++] = this->_vertices[i].getVertex()._z;
				} else {
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
				}
				if (i < this->_verticesNorm.size()) {
					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._x;
					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._y;
					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._z;
				} else {
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
				}
				break;
			case VERTEX_TEXT_VNORM:
				if (i < this->_vertices.size()) {
					vbo->data[posInsert++] = this->_vertices[i].getVertex()._x;
					vbo->data[posInsert++] = this->_vertices[i].getVertex()._y;
					vbo->data[posInsert++] = this->_vertices[i].getVertex()._z;
				} else {
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
				}
				if (i < this->_textureCoors.size()) {
					vbo->data[posInsert++] = this->_textureCoors[i].getTexture()._x;
					vbo->data[posInsert++] = this->_textureCoors[i].getTexture()._y;
				} else {
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
				}
				if (i < this->_verticesNorm.size()) {
					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._x;
					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._y;
					vbo->data[posInsert++] = this->_verticesNorm[i].getVertexNorm()._z;
				} else {
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
					vbo->data[posInsert++] = 0.0f;
				}
				break;
		}
	}
	return vbo;
}

std::shared_ptr<EBO> ParsedData::createEBO( FaceType type ) const {
	if (this->_faces.count(type)== 0)
		throw AppException("No faces of type " + faceToString(type));

	std::shared_ptr<EBO> ebo = std::make_shared<EBO>();
	ebo->size = this->_faces.at(type).size();
	ebo->type = type;
	unsigned int posInsert = 0;

	switch (type) {
		case VERTEX:
			ebo->stride = 3;
			ebo->data = std::make_unique<unsigned int[]>(ebo->size * ebo->stride);
			for (Face const& face : this->_faces.at(VERTEX)) {
				for (t_index3D const& vertexIndex : face.getCoors())
					ebo->data[posInsert++] = vertexIndex._i1 - 1;
			}
			break;
		case VERTEX_TEXT:
			ebo->stride = 6;
			ebo->data = std::make_unique<unsigned int[]>(ebo->size * ebo->stride);
			for (Face const& face : this->_faces.at(VERTEX_TEXT)) {
				for (t_index3D const& vertexTextIndex : face.getCoors()) {
					ebo->data[posInsert++] = vertexTextIndex._i1 - 1;
					ebo->data[posInsert++] = vertexTextIndex._i2 - 1;
				}
			}
			break;
		case VERTEX_VNORM:
			ebo->stride = 6;
			ebo->data = std::make_unique<unsigned int[]>(ebo->size * ebo->stride);
			for (Face const& face : this->_faces.at(VERTEX_VNORM)) {
				for (t_index3D const& vertexNormIndex : face.getCoors()) {
					ebo->data[posInsert++] = vertexNormIndex._i1 - 1;
					ebo->data[posInsert++] = vertexNormIndex._i2 - 1;
				}
			}
			break;
		case VERTEX_TEXT_VNORM:
			ebo->stride = 9;
			ebo->data = std::make_unique<unsigned int[]>(ebo->size * ebo->stride);
			for (Face const& face : this->_faces.at(VERTEX_TEXT_VNORM)) {
				for (t_index3D const& vertexTextNormIndex : face.getCoors()) {
					ebo->data[posInsert++] = vertexTextNormIndex._i1 - 1;
					ebo->data[posInsert++] = vertexTextNormIndex._i2 - 1;
					ebo->data[posInsert++] = vertexTextNormIndex._i3 - 1;
				}
			}
			break;
	}
	return ebo;
}


std::ostream& operator<<(std::ostream& os, VertexCoor const& obj) {
	t_coor3D vertexCoor = obj.getVertex();
	os << vertexCoor._x << " " << vertexCoor._y << " " << vertexCoor._z;
	return os;
}

std::ostream& operator<<(std::ostream& os, TextureCoor const& obj) {
	t_coor3D textureCoor = obj.getTexture();
	os << textureCoor._x << " " << textureCoor._y;
	if (textureCoor._z != .0f)
		os << " " << textureCoor._z;
	return os;
}

std::ostream& operator<<(std::ostream& os, VertexNormCoor const& obj) {
	t_coor3D vertexNormCoor = obj.getVertexNorm();
	os << vertexNormCoor._x << " " << vertexNormCoor._y << " " << vertexNormCoor._z;
	return os;
}

std::ostream& operator<<(std::ostream& os, VertexSpaceParamCoor const& obj) {
	t_coor3D vertexSpaceCoor = obj.getVertexSpaceParam();
	os << vertexSpaceCoor._x;
	if (vertexSpaceCoor._y != .0f)
		os << " " << vertexSpaceCoor._y;
	if (vertexSpaceCoor._z != .0f)
		os << " " << vertexSpaceCoor._z;
	return os;
}

std::ostream& operator<<( std::ostream& os, FaceType type ) {
	os << faceToString(type);
	return os;
}

std::ostream& operator<<(std::ostream& os, Face const& obj) {
	switch (obj.getFaceType()) {
		case (VERTEX):
			for (t_index3D const& faceIndex : obj.getCoors()) {
				os << faceIndex._i1 << " ";
			}
			break;
		case (VERTEX_TEXT):
			for (t_index3D const& faceIndex : obj.getCoors()) {
				os << faceIndex._i1 << "/" << faceIndex._i2 << " ";
			}
			break;
		case (VERTEX_VNORM):
			for (t_index3D const& faceIndex : obj.getCoors()) {
				os << faceIndex._i1 << "//" << faceIndex._i2 << " ";
			}
			break;
		case (VERTEX_TEXT_VNORM):
			for (t_index3D const& faceIndex : obj.getCoors()) {
				os << faceIndex._i1 << "/" << faceIndex._i2 << "/" << faceIndex._i3 << " ";
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
	for (unsigned int lineIndex : obj.getCoors()) {
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
	for (unsigned int i=0; i<data.size; i++) {
		for (unsigned int j=0; j < data.stride; j++)
			os << data.getData()[i * data.stride + j] << " ";
		os << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, EBO const& data) {
	unsigned int const* indexes = data.getData();

	for (unsigned int i=0; i<data.size; i++) {
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
	for (VertexCoor const& vertex : obj.getVertices())
		os << "v: " << vertex << std::endl;
	for (TextureCoor const& texture : obj.getTextureCoors())
		os << "vt: " << texture << std::endl;
	for (VertexNormCoor const& vertexNorm : obj.getVerticesNorm())
		os << "vn: " << vertexNorm << std::endl;
	for (VertexSpaceParamCoor const& vertexParam : obj.getParamSpaceVertices())
		os << "vp: " << vertexParam << std::endl;
	for (const auto& [key, faces] : obj.getFaces()) {
		for (Face const& face : faces)
			os << "f: " << face << std::endl;
	}
	for (Line const& line : obj.getLines())
		os << "l: " << line << std::endl;
	return os;
}
