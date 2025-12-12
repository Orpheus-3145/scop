#include "parser/parserTypes.hpp"


coor2D::coor2D( coor2D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
}

coor2D::coor2D( std::vector<double> const& coors) {
	if (coors.size() < 2)
		throw ParsingException("Invalid size vector, has to be 2");
	this->_x = coors[0];
	this->_y = coors[1];
}

coor2D& coor2D::operator=(coor2D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	return *this;
}


coor3D::coor3D( coor3D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	this->_z = coor._z;
}

coor3D::coor3D( std::vector<double> const& coors) {
	if (coors.size() < 3)
		throw ParsingException("Invalid size vector, has to be 3");
	this->_x = coors[0];
	this->_y = coors[1];
	this->_z = coors[2];
}

coor3D& coor3D::operator=(coor3D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	this->_z = coor._z;
	return *this;
}


coor4D::coor4D( coor4D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	this->_z = coor._z;
	this->_w = coor._w;
}

coor4D::coor4D( std::vector<double> const& coors) {
	if (coors.size() < 4)
		throw ParsingException("Invalid size vector, has to be 4");
	this->_x = coors[0];
	this->_y = coors[1];
	this->_z = coors[2];
	this->_w = coors[3];
}

coor4D& coor4D::operator=(coor4D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	this->_z = coor._z;
	this->_w = coor._w;
	return *this;
}


index2D::index2D( index2D const& coor) noexcept {
	this->_i1 = coor._i1;
	this->_i2 = coor._i2;
}

index2D::index2D( std::vector<unsigned int> const& positions ) {
	if (positions.size() < 1)
		throw ParsingException("Not enough index provided");
	this->_i1 = positions[0];
	this->_i2 = positions[1];
}

index2D& index2D::operator=(index2D const& coor) noexcept {
	this->_i1 = coor._i1;
	this->_i2 = coor._i2;
	return *this;
}


index3D::index3D( index3D const& coor) noexcept {
	this->_i1 = coor._i1;
	this->_i2 = coor._i2;
	this->_i3 = coor._i3;
}

index3D::index3D( std::vector<unsigned int> const& positions ) {
	if (positions.size() < 2)
		throw ParsingException("Not enough index provided");
	this->_i1 = positions[0];
	this->_i2 = positions[1];
	this->_i3 = positions[2];
}

index3D& index3D::operator=(index3D const& coor) noexcept {
	this->_i1 = coor._i1;
	this->_i2 = coor._i2;
	this->_i3 = coor._i3;
	return *this;
}


VertexCoor::VertexCoor( std::vector<double> const& coorVect ) {
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

TextureCoor::TextureCoor( std::vector<double> const& coorVect ) {
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

VertexNormCoor::VertexNormCoor( std::vector<double> const& coorVect ) {
	if (coorVect.size() != 3)
		throw ParsingException("Invalid size for vector to initialize a VertexNorm instance, has to be 3");

	this->_coor = t_coor3D(coorVect[0], coorVect[1], coorVect[2]);
}

t_coor3D const& VertexNormCoor::getVertexNorm( void ) const noexcept {
	return this->_coor;
}

VertexSpaceParamCoor::VertexSpaceParamCoor( std::vector<double> const& coorVect ) {
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


FaceCoor::FaceCoor( std::vector<unsigned int> const& indexVect ) {
	switch(indexVect.size()) {
		case 1:
			this->_index = t_index3D(indexVect[0], 0, 0);
			break;
		case 2:
			this->_index = t_index3D(indexVect[0], indexVect[1], 0);
			break;
		case 3:
			this->_index = t_index3D(indexVect[0], indexVect[1], indexVect[2]);
			break;
		default:
			throw ParsingException("Invalid size for vector to initialize a FaceCoor instance, has to be 1, 2 or 3");
	}
}

t_index3D const& FaceCoor::getIndex( void ) const noexcept {
	return this->_index;
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

void Face::setSmoothing( int newSmoothing ) noexcept {
	this->_smoothing = newSmoothing;
}

FaceType Face::getFaceType( void ) const noexcept {
	return this->_type;
}

std::vector<FaceCoor> Face::getCoors( void ) const noexcept {
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

int Face::getSmoothing( void ) const noexcept {
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

void Line::setSmoothing( int newSmoothing ) noexcept {
	this->_smoothing = newSmoothing;
}

std::vector<unsigned int> Line::getCoors( void ) const noexcept {
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

int Line::getSmoothing( void ) const noexcept {
	return this->_smoothing;
}


std::vector<std::string> const& ObjData::getTmlFiles( void ) const noexcept {
	return this->_tmlFiles;
}

std::vector<VertexCoor> const& ObjData::getVertices( void ) const noexcept {
	return this->_vertices;
}

std::vector<TextureCoor> const& ObjData::getTextureCoors( void ) const noexcept {
	return this->_textureCoors;
}

std::vector<VertexNormCoor> const& ObjData::getVerticesNorm( void ) const noexcept {
	return this->_verticesNorm;
}

std::vector<VertexSpaceParamCoor> const& ObjData::getParamSpaceVertices( void ) const noexcept {
	return this->_paramSpaceVertices;
}

std::vector<Face> const& ObjData::getFaces( void ) const noexcept {
	return this->_faces;
}

std::vector<Line> const& ObjData::getLines( void ) const noexcept {
	return this->_lines;
}

void ObjData::addTmlFile( std::string const& newFile ) noexcept {
	this->_tmlFiles.push_back(newFile);
}

void ObjData::addVertex( VertexCoor const& newVertex ) {
	this->_vertices.push_back(newVertex);
}

void ObjData::addTextureCoor( TextureCoor const& newTexture ) {
	this->_textureCoors.push_back(newTexture);
}

void ObjData::addVertexNorm( VertexNormCoor const& newVertexNorm ) {
	this->_verticesNorm.push_back(newVertexNorm);
}

void ObjData::addParamSpaceVertex( VertexSpaceParamCoor const& newVertexParam ) {
	this->_paramSpaceVertices.push_back(newVertexParam);
}

void ObjData::addFace( Face const& newFace ) noexcept {
	this->_faces.push_back(newFace);
}

void ObjData::addLine( Line const& newLine ) noexcept {
	this->_lines.push_back(newLine);
}

RawData<double> ObjData::getVertexData( void ) const noexcept {
	unsigned int size = this->_vertices.size();
	if (size == 0)
		return RawData<double>();

	unsigned int dimension = 3;
	std::unique_ptr<double[]> data = std::make_unique<double[]>(size * dimension);
	for(size_t i=0; i<size; i++) {
		t_coor3D const coor = this->_vertices[i].getVertex();
		data[i] = coor._x;
		data[i + 1] = coor._y;
		data[i + 2] = coor._z;
	}
	return RawData<double>(data, size, dimension);
}

// void ObjData::getTextureData( std::unique_ptr<double[]>& data, unsigned int& size, unsigned int& dimension ) const noexcept {
// 	if (this->_textureCoors.size() == 0) {
// 		size = 0;
// 		dimension = 0;
// 		return;
// 	}
// 
// 	if (std::holds_alternative<double>(this->_textureCoors[0].getTexture())) {
// 		size = this->_textureCoors.size();
// 		dimension = 1;
// 		data = std::make_unique<double[]>(size * dimension);
// 		for(size_t i=0; i<this->_textureCoors.size(); i++) {
// 			double coor = std::get<double>(this->_textureCoors[i].getTexture());
// 			data[i] = coor;
// 		}
// 	} else if (std::holds_alternative<t_coor2D>(this->_textureCoors[0].getTexture())) {
// 		size = this->_textureCoors.size();
// 		dimension = 2;
// 		data = std::make_unique<double[]>(size * dimension);
// 		for(size_t i=0; i<this->_textureCoors.size(); i++) {
// 			t_coor2D coor = std::get<t_coor2D>(this->_textureCoors[i].getTexture());
// 			data[i] = coor._x;
// 			data[i + 1] = coor._y;
// 		}
// 	} else if (std::holds_alternative<t_coor3D>(this->_textureCoors[0].getTexture())) {
// 		size = this->_textureCoors.size();
// 		dimension = 3;
// 		data = std::make_unique<double[]>(size * dimension);
// 		for(size_t i=0; i<this->_textureCoors.size(); i++) {
// 			t_coor3D coor = std::get<t_coor3D>(this->_textureCoors[i].getTexture());
// 			data[i] = coor._x;
// 			data[i + 1] = coor._y;
// 			data[i + 2] = coor._z;
// 		}
// 	}
// }

// void ObjData::getVertexNormData( std::unique_ptr<double[]>& data, unsigned int& size, unsigned int& dimension ) const noexcept {
// 	if (this->_verticesNorm.size() == 0) {
// 		size = 0;
// 		dimension = 0;
// 		return;
// 	}
// 
// 	size = this->_verticesNorm.size();
// 	dimension = 3;
// 	data = std::make_unique<double[]>(size * dimension);
// 	for(size_t i=0; i<this->_verticesNorm.size(); i++) {
// 		t_coor3D coor = this->_verticesNorm[i].getVertexNorm();
// 		data[i] = coor._x;
// 		data[i + 1] = coor._y;
// 		data[i + 2] = coor._z;
// 	}
// }

// void ObjData::getParamSpaceVertexData( std::unique_ptr<double[]>& data, unsigned int& size, unsigned int& dimension ) const noexcept {
// 	if (this->_paramSpaceVertices.size() == 0) {
// 		size = 0;
// 		dimension = 0;
// 		return;
// 	}
// 
// 	if (std::holds_alternative<double>(this->_paramSpaceVertices[0].getVertexSpaceParam())) {
// 		size = this->_paramSpaceVertices.size();
// 		dimension = 1;
// 		data = std::make_unique<double[]>(size * dimension);
// 		for(size_t i=0; i<this->_paramSpaceVertices.size(); i++) {
// 			double coor = std::get<double>(this->_paramSpaceVertices[i].getVertexSpaceParam());
// 			data[i] = coor;
// 		}
// 	} else if (std::holds_alternative<t_coor2D>(this->_paramSpaceVertices[0].getVertexSpaceParam())) {
// 		size = this->_paramSpaceVertices.size();
// 		dimension = 2;
// 		data = std::make_unique<double[]>(size * dimension);
// 		for(size_t i=0; i<this->_paramSpaceVertices.size(); i++) {
// 			t_coor2D coor = std::get<t_coor2D>(this->_paramSpaceVertices[i].getVertexSpaceParam());
// 			data[i] = coor._x;
// 			data[i + 1] = coor._y;
// 		}
// 	} else if (std::holds_alternative<t_coor3D>(this->_paramSpaceVertices[0].getVertexSpaceParam())) {
// 		size = this->_paramSpaceVertices.size();
// 		dimension = 3;
// 		data = std::make_unique<double[]>(size * dimension);
// 		for(size_t i=0; i<this->_paramSpaceVertices.size(); i++) {
// 			t_coor3D coor = std::get<t_coor3D>(this->_paramSpaceVertices[i].getVertexSpaceParam());
// 			data[i] = coor._x;
// 			data[i + 1] = coor._y;
// 			data[i + 2] = coor._z;
// 		}
// 	}
// }

VertexCoor const& ObjData::getindexVertex( unsigned int index ) const {
	try {
		return this->_vertices.at(index);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("Vertex index: " + std::to_string(index) + " out of bounds");
	}
}

TextureCoor const& ObjData::getindexTexture( unsigned int index ) const {
	try {
		return this->_textureCoors.at(index);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("Texture index: " + std::to_string(index) + " out of bounds");
	}
}

VertexNormCoor const& ObjData::getindexVertexNorm( unsigned int index ) const {
	try {
		return this->_verticesNorm.at(index);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("VertexNorm index: " + std::to_string(index) + " out of bounds");
	}
}

VertexSpaceParamCoor const& ObjData::getindexVertexSpaceParam( unsigned int index ) const {
	try {
		return this->_paramSpaceVertices.at(index);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("VertexSpaceParam: " + std::to_string(index) + " out of bounds");
	}
}


std::ostream& operator<<(std::ostream& os, const VertexCoor& obj) {
	t_coor3D vertexCoor = obj.getVertex();
	os << vertexCoor._x << " " << vertexCoor._y << " " << vertexCoor._z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const TextureCoor& obj) {
	t_coor3D textureCoor = obj.getTexture();
	os << textureCoor._x << " " << textureCoor._y;
	if (textureCoor._z != .0f)
		os << " " << textureCoor._z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const VertexNormCoor& obj) {
	t_coor3D vertexNormCoor = obj.getVertexNorm();
	os << vertexNormCoor._x << " " << vertexNormCoor._y << " " << vertexNormCoor._z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const VertexSpaceParamCoor& obj) {
	t_coor3D vertexSpaceCoor = obj.getVertexSpaceParam();
	os << vertexSpaceCoor._x;
	if (vertexSpaceCoor._y != .0f)
		os << " " << vertexSpaceCoor._y;
	if (vertexSpaceCoor._z != .0f)
		os << " " << vertexSpaceCoor._z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Face& obj) {
	switch (obj.getFaceType()) {
		case (VERTEX):
			for (FaceCoor const& faceIndex : obj.getCoors()) {
				t_index3D index(faceIndex.getIndex());
				os << index._i1 << " ";
			}
			break;
		case (VERTEX_TEXT):
			for (FaceCoor const& faceIndex : obj.getCoors()) {
				t_index3D index(faceIndex.getIndex());
				os << index._i1 << "/" << index._i2 << " ";
			}
			break;
		case (VERTEX_VNORM):
			for (FaceCoor const& faceIndex : obj.getCoors()) {
				t_index3D index(faceIndex.getIndex());
				os << index._i1 << "//" << index._i2 << " ";
			}
			break;
		case (VERTEX_TEXT_VNORM):
			for (FaceCoor const& faceIndex : obj.getCoors()) {
				t_index3D index(faceIndex.getIndex());
				os << index._i1 << "/" << index._i2 << "/" << index._i3 << " ";
			}
			break;
	}
	if (obj.getObject().length() > 0)
		os << "|object: " << obj.getObject() << "|";
	if (obj.getGroup().length() > 0)
		os << "|group: " << obj.getGroup() << "|";
	if (obj.getMaterial().length() > 0)
		os << "|material: " << obj.getMaterial() << "|";
	if (obj.getSmoothing() != -1)
		os << "|smoothing: " << obj.getSmoothing() << "|";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Line& obj) {
	for (unsigned int lineIndex : obj.getCoors()) {
		os << lineIndex << " ";
	}
	if (obj.getObject().length() > 0)
		os << "|object: " << obj.getObject() << "|";
	if (obj.getGroup().length() > 0)
		os << "|group: " << obj.getGroup() << "|";
	if (obj.getMaterial().length() > 0)
		os << "|material: " << obj.getMaterial() << "|";
	if (obj.getSmoothing() != -1)
		os << "|smoothing: " << obj.getSmoothing() << "|";
	return os;
}

std::ostream& operator<<(std::ostream& os, const ObjData& obj) {
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
	for (Face const& face : obj.getFaces())
		os << "f: " << face << std::endl;
	for (Line const& line : obj.getLines())
		os << "l: " << line << std::endl;
	return os;
}
