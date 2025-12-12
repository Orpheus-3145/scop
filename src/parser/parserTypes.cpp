#include "parser/parserTypes.hpp"


coor3D::coor3D( coor3D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	this->_z = coor._z;
}

coor3D::coor3D( std::vector<double> const& coors) {
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

int Line::getSmoothing( void ) const noexcept {
	return this->_smoothing;
}


RawData::RawData( RawData const& other ) noexcept {
	if (this != &other) {
		this->setCoors(other._coor, other._nCoors);
		this->setIndex(other._index, other._nIndex);
	}
}

RawData::RawData( RawData&& other ) noexcept {
	if (this != &other) {
		this->setCoors(std::move(other._coor), other._nCoors);
		this->setIndex(std::move(other._index), other._nIndex);
	}
}

RawData& RawData::operator=(RawData const& other ) {
	if (this != &other) {
		this->setCoors(other._coor, other._nCoors);
		this->setIndex(other._index, other._nIndex);
	}
	return *this;
}

RawData& RawData::operator=(RawData&& other ) {
	if (this != &other) {
		this->setCoors(std::move(other._coor), other._nCoors);
		this->setIndex(std::move(other._index), other._nIndex);
	}
	return *this;
}

void	RawData::setCoors(std::unique_ptr<double[]> const& coor, unsigned int size) noexcept {
	this->_nCoors = size;
	unsigned int totSize = this->_nCoors * 3 /*v-vt-vn*/ * 3 /* three double for coor*/;

	this->_coor.reset(new double[totSize]);
	std::copy(coor.get(), coor.get() + totSize, this->_coor.get());
}

void	RawData::setCoors(std::unique_ptr<double[]>&& coor, unsigned int size) noexcept {
	this->_nCoors = size;
	this->_coor = std::move(coor);
}

void RawData::setIndex(std::unique_ptr<unsigned int[]> const& indexes, unsigned int size) noexcept {
	this->_nIndex = size;
	unsigned int totSize = this->_nIndex * 3 /*v-vt-vn*/ * 3 /* three unsigned int for three vertex*/;
	this->_index.reset(new unsigned int[totSize]);
	std::copy(indexes.get(), indexes.get() + totSize, this->_index.get());
}

void RawData::setIndex(std::unique_ptr<unsigned int[]>&& indexes, unsigned int size) noexcept {
	this->_nIndex = size;
	this->_index = std::move(indexes);
}

double*	RawData::getCoors( void ) const noexcept {
	return _coor.get();
};

unsigned int*  RawData::getIndex( void ) const noexcept {
	return this->_index.get();
}

unsigned int RawData::getNcoors( void ) const noexcept {
	return _nCoors;
};

unsigned int RawData::getNindex( void ) const noexcept {
	return this->_nIndex;
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

RawData ObjData::getVertexData( void ) const {
	RawData data;
	unsigned int maxLenghtCoor = std::max({this->_vertices.size() + this->_textureCoors.size() + this->_verticesNorm.size()});
	unsigned int size = maxLenghtCoor * 3 * 3;
	
	std::unique_ptr<double[]> coorData = std::make_unique<double[]>(size);
	double* tmp1 = coorData.get();
	
	for (unsigned int i=0; i<size; i++) {
		if (i < this->_vertices.size()) {
			t_coor3D const coor = this->_vertices[i].getVertex();
			tmp1[i] = coor._x;
			tmp1[i + 1] = coor._y;
			tmp1[i + 2] = coor._z;
		}
		if (i < this->_textureCoors.size()) {
			t_coor3D const coor = this->_textureCoors[i].getTexture();
			tmp1[i + 3] = coor._x;
			tmp1[i + 4] = coor._y;
			tmp1[i + 5] = coor._z;
		}
		if (i < this->_verticesNorm.size()) {
			t_coor3D const coor = this->_verticesNorm[i].getVertexNorm();
			tmp1[i + 6] = coor._x;
			tmp1[i + 7] = coor._y;
			tmp1[i + 8] = coor._z;
		}
	}
	data.setCoors(std::move(coorData), maxLenghtCoor);

	size = this->_faces.size() * 3 * 3;
	std::unique_ptr<unsigned int[]> indexData = std::make_unique<unsigned int[]>(size);
	unsigned int* tmp2 = indexData.get();
	for (unsigned int i=0; i<this->_faces.size(); i++) {
		std::vector<t_index3D> coors = this->_faces[i].getCoors();
		if (coors.size() != 3)
			throw ParsingException("Faces with a different number than 3 of index, apply ear clipping first");
		for (unsigned int j=0; j<coors.size(); j++) {
			tmp2[i + j] = coors[j]._i1;
			tmp2[i + j + 1] = coors[j]._i2;
			tmp2[i + j + 2] = coors[j]._i3;
		}
	}
	data.setIndex(std::move(indexData), this->_faces.size());
	return data;
}

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
