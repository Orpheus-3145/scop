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


RawData::RawData( void ) noexcept {
	this->_nCoors = 0;
	this->_coorsType = VERTEX;

	this->_nIndexes[VERTEX] = 0;
	this->_nIndexes[VERTEX_TEXT] = 0;
	this->_nIndexes[VERTEX_VNORM] = 0;
	this->_nIndexes[VERTEX_TEXT_VNORM] = 0;
}

RawData::RawData( RawData const& other ) noexcept {
	if (this != &other) {
		this->_nCoors = other._nCoors;
		this->_coorsType = other._coorsType;
		unsigned int totSize = this->_nCoors * this->getStride() * 3;

		this->_coor = std::make_unique<double[]>(totSize);
		std::copy(other._coor.get(), other._coor.get() + totSize, this->_coor.get());

		this->_nIndexes = other._nIndexes;
		this->_indexes[VERTEX] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX]);
		this->_indexes[VERTEX_TEXT] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX_TEXT]);
		this->_indexes[VERTEX_VNORM] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX_VNORM]);
		this->_indexes[VERTEX_TEXT_VNORM] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX_TEXT_VNORM]);

		std::copy(other._indexes.at(VERTEX).get(), other._indexes.at(VERTEX).get() + this->_nIndexes.at(VERTEX), this->_indexes.at(VERTEX).get());
		std::copy(other._indexes.at(VERTEX_TEXT).get(), other._indexes.at(VERTEX_TEXT).get() + this->_nIndexes.at(VERTEX_TEXT), this->_indexes.at(VERTEX_TEXT).get());
		std::copy(other._indexes.at(VERTEX_VNORM).get(), other._indexes.at(VERTEX_VNORM).get() + this->_nIndexes.at(VERTEX_VNORM), this->_indexes.at(VERTEX_VNORM).get());
		std::copy(other._indexes.at(VERTEX_TEXT_VNORM).get(), other._indexes.at(VERTEX_TEXT_VNORM).get() + this->_nIndexes.at(VERTEX_TEXT_VNORM), this->_indexes.at(VERTEX_TEXT_VNORM).get());
	}
}

RawData::RawData( RawData&& other ) noexcept {
	if (this != &other) {
		this->_nCoors = other._nCoors;
		this->_coorsType = other._coorsType;
		this->_coor = std::move(other._coor);

		this->_nIndexes = other._nIndexes;
		this->_indexes[VERTEX] = std::move(other._indexes.at(VERTEX));
		this->_indexes[VERTEX_TEXT] = std::move(other._indexes.at(VERTEX_TEXT));
		this->_indexes[VERTEX_VNORM] = std::move(other._indexes.at(VERTEX_VNORM));
		this->_indexes[VERTEX_TEXT_VNORM] = std::move(other._indexes.at(VERTEX_TEXT_VNORM));
	}
}

RawData& RawData::operator=(RawData const& other ) {
	if (this != &other) {
		this->_nCoors = other._nCoors;
		this->_coorsType = other._coorsType;
		unsigned int totSize = this->_nCoors * this->getStride() * 3;

		this->_coor = std::make_unique<double[]>(totSize);
		std::copy(other._coor.get(), other._coor.get() + totSize, this->_coor.get());

		this->_nIndexes = other._nIndexes;
		this->_indexes[VERTEX] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX]);
		this->_indexes[VERTEX_TEXT] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX_TEXT]);
		this->_indexes[VERTEX_VNORM] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX_VNORM]);
		this->_indexes[VERTEX_TEXT_VNORM] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX_TEXT_VNORM]);

		std::copy(other._indexes.at(VERTEX).get(), other._indexes.at(VERTEX).get() + this->_nIndexes.at(VERTEX), this->_indexes.at(VERTEX).get());
		std::copy(other._indexes.at(VERTEX_TEXT).get(), other._indexes.at(VERTEX_TEXT).get() + this->_nIndexes.at(VERTEX_TEXT), this->_indexes.at(VERTEX_TEXT).get());
		std::copy(other._indexes.at(VERTEX_VNORM).get(), other._indexes.at(VERTEX_VNORM).get() + this->_nIndexes.at(VERTEX_VNORM), this->_indexes.at(VERTEX_VNORM).get());
		std::copy(other._indexes.at(VERTEX_TEXT_VNORM).get(), other._indexes.at(VERTEX_TEXT_VNORM).get() + this->_nIndexes.at(VERTEX_TEXT_VNORM), this->_indexes.at(VERTEX_TEXT_VNORM).get());
	}
	return *this;
}

RawData& RawData::operator=(RawData&& other ) {
	if (this != &other) {
		this->_nCoors = other._nCoors;
		this->_coorsType = other._coorsType;
		this->_coor = std::move(other._coor);

		this->_nIndexes = other._nIndexes;
		this->_indexes[VERTEX] = std::move(other._indexes.at(VERTEX));
		this->_indexes[VERTEX_TEXT] = std::move(other._indexes.at(VERTEX_TEXT));
		this->_indexes[VERTEX_VNORM] = std::move(other._indexes.at(VERTEX_VNORM));
		this->_indexes[VERTEX_TEXT_VNORM] = std::move(other._indexes.at(VERTEX_TEXT_VNORM));
	}
	return *this;
}
// NB: rawData aka VBO has to be built reading t
void RawData::setCoors( std::vector<VertexCoor> const& vertexes, std::vector<TextureCoor> const& textures, std::vector<VertexNormCoor> const& vertexesNorm ) noexcept {
	this->_nCoors = std::max({vertexes.size(), textures.size(), vertexesNorm.size()});
	if (textures.size() > 0) {
		if (vertexesNorm.size() > 0)
			this->_coorsType = VERTEX_TEXT_VNORM;
		else
			this->_coorsType = VERTEX_TEXT;
	} else if (vertexesNorm.size() > 0)
		this->_coorsType = VERTEX_VNORM;
	else
		this->_coorsType = VERTEX;

	// to optmize, the array has 3 (dimension) * stride (1: only vertex, 2: vertex/texture, ...) columns instead of the maximum (9)
	// stride = 1: |vx|vy|vz|
	// stride = 2: |vx|vy|vz|vtx|vty|vtz| or |vx|vy|vz|vnx|vny|vnz|
	// stride = 3: |vx|vy|vz|vtx|vty|vtz|vnx|vny|vnz|
	unsigned int sizeSlot = this->getStride() * 3; // 3 hard-coded beacause only 3D arrays are used
	this->_coor = std::make_unique<double[]>(this->_nCoors * sizeSlot);
	for (unsigned int i=0; i<this->_nCoors; i++) {
		switch (this->_coorsType) {
			case VERTEX:
				this->_coor[i * sizeSlot] = vertexes[i].getVertex()._x;
				this->_coor[i * sizeSlot + 1] = vertexes[i].getVertex()._y;
				this->_coor[i * sizeSlot + 2] = vertexes[i].getVertex()._z;
				break;
			case VERTEX_TEXT:
				if (i < vertexes.size()) {
					this->_coor[i * sizeSlot] = vertexes[i].getVertex()._x;
					this->_coor[i * sizeSlot + 1] = vertexes[i].getVertex()._y;
					this->_coor[i * sizeSlot + 2] = vertexes[i].getVertex()._z;
				}
				if (i < textures.size()) {
					this->_coor[i * sizeSlot + 3] = textures[i].getTexture()._x;
					this->_coor[i * sizeSlot + 4] = textures[i].getTexture()._y;
					this->_coor[i * sizeSlot + 5] = textures[i].getTexture()._z;
				}
				break;
			case VERTEX_VNORM:
				if (i < vertexes.size()) {
					this->_coor[i * sizeSlot] = vertexes[i].getVertex()._x;
					this->_coor[i * sizeSlot + 1] = vertexes[i].getVertex()._y;
					this->_coor[i * sizeSlot + 2] = vertexes[i].getVertex()._z;
				}
				if (i < vertexesNorm.size()) {
					this->_coor[i * sizeSlot + 3] = vertexesNorm[i].getVertexNorm()._x;
					this->_coor[i * sizeSlot + 4] = vertexesNorm[i].getVertexNorm()._y;
					this->_coor[i * sizeSlot + 5] = vertexesNorm[i].getVertexNorm()._z;
				}
				break;
			case VERTEX_TEXT_VNORM:
				if (i < vertexes.size()) {
					this->_coor[i * sizeSlot] = vertexes[i].getVertex()._x;
					this->_coor[i * sizeSlot + 1] = vertexes[i].getVertex()._y;
					this->_coor[i * sizeSlot + 2] = vertexes[i].getVertex()._z;
				}
				if (i < textures.size()) {
					this->_coor[i * sizeSlot + 3] = textures[i].getTexture()._x;
					this->_coor[i * sizeSlot + 4] = textures[i].getTexture()._y;
					this->_coor[i * sizeSlot + 5] = textures[i].getTexture()._z;
				}
				if (i < vertexesNorm.size()) {
					this->_coor[i * sizeSlot + 6] = vertexesNorm[i].getVertexNorm()._x;
					this->_coor[i * sizeSlot + 7] = vertexesNorm[i].getVertexNorm()._y;
					this->_coor[i * sizeSlot + 8] = vertexesNorm[i].getVertexNorm()._z;
				}
				break;
		}
	}
}

void RawData::setIndexes(std::vector<Face> const& faces) noexcept {
	for(Face const& face : faces)
		this->_nIndexes[face.getFaceType()] += 1;
	// format array: |i(v)x|i(v)y|i(v)z|
	this->_indexes[VERTEX] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX] * 3);
	// format array: |i(v)x|i(v)y|i(v)z|i(vt)x|i(vt)y|i(vt)z|
	this->_indexes[VERTEX_TEXT] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX_TEXT] * 6);
	// format array: |i(v)x|i(v)y|i(v)z|i(vn)x|i(vn)y|i(vn)z|
	this->_indexes[VERTEX_VNORM] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX_VNORM] * 6);
	// format array: |i(v)x|i(v)y|i(v)z|i(vt)x|i(vt)y|i(vt)z|i(vn)x|i(vn)y|i(vn)z|
	this->_indexes[VERTEX_TEXT_VNORM] = std::make_unique<unsigned int []>(this->_nIndexes[VERTEX_TEXT_VNORM] * 9);

	unsigned int *vertPos = this->_indexes[VERTEX].get();
	unsigned int *vertTextPos = this->_indexes[VERTEX_TEXT].get();
	unsigned int *vertNormPos = this->_indexes[VERTEX_VNORM].get();
	unsigned int *vertTextNormPos = this->_indexes[VERTEX_TEXT_VNORM].get();

	for(Face const& face : faces) {
		for (t_index3D const& coor : face.getCoors()) {
			switch (face.getFaceType()) {
				case VERTEX:
					*vertPos++ = coor._i1;
					break;
				case VERTEX_TEXT:
					*vertTextPos++ = coor._i1;
					*vertTextPos++ = coor._i2;
					break;
				case VERTEX_VNORM:
					*vertNormPos++ = coor._i1;
					*vertNormPos++ = coor._i2;
					break;
				case VERTEX_TEXT_VNORM:
					*vertTextNormPos++ = coor._i1;
					*vertTextNormPos++ = coor._i2;
					*vertTextNormPos++ = coor._i3;
					break;
			}
		}
	}
}

unsigned int RawData::getNcoors( void ) const noexcept {
	return this->_nCoors;
};

FaceType RawData::getType( void ) const noexcept {
	return this->_coorsType;
}

unsigned int RawData::getStride( void ) const noexcept {
	switch (this->_coorsType) {
		case VERTEX:
			return 1;
		case VERTEX_TEXT:
			return 2;
		case VERTEX_VNORM:
			return 2;
		case VERTEX_TEXT_VNORM:
			return 3;
		default:
			return 0;
	}
}

double*	RawData::getCoors( void ) const noexcept {
	return this->_coor.get();
};

unsigned int RawData::getNindex( FaceType type ) const noexcept {
	return this->_nIndexes.at(type);
}

unsigned int* RawData::getIndex( FaceType type ) const noexcept {
	return this->_indexes.at(type).get();
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

std::vector<Face> const& ParsedData::getFaces( void ) const noexcept {
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
	this->_faces.push_back(newFace);
}

void ParsedData::addLine( Line const& newLine ) noexcept {
	this->_lines.push_back(newLine);
}

std::unique_ptr<RawData> ParsedData::getData( void ) const {
	std::unique_ptr<RawData> data = std::make_unique<RawData>();
	data->setCoors(this->_vertices, this->_textureCoors, this->_verticesNorm);
	// NB apply ear clipping / fan triangulation so every face has exactly 3 groups of indexes
	// this method only works if every face has exactly 3 coors
	data->setIndexes(this->_faces);
	return data;
}

VertexCoor const& ParsedData::getindexVertex( unsigned int index ) const {
	try {
		return this->_vertices.at(index);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("Vertex index: " + std::to_string(index) + " out of bounds");
	}
}

TextureCoor const& ParsedData::getindexTexture( unsigned int index ) const {
	try {
		return this->_textureCoors.at(index);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("Texture index: " + std::to_string(index) + " out of bounds");
	}
}

VertexNormCoor const& ParsedData::getindexVertexNorm( unsigned int index ) const {
	try {
		return this->_verticesNorm.at(index);
	}
	catch (std::out_of_range const& e) {
		throw ParsingException("VertexNorm index: " + std::to_string(index) + " out of bounds");
	}
}

VertexSpaceParamCoor const& ParsedData::getindexVertexSpaceParam( unsigned int index ) const {
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
	if (obj.getSmoothing() != 0)
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
	if (obj.getSmoothing() != 0)
		os << "|smoothing: " << obj.getSmoothing() << "|";
	return os;
}

std::ostream& operator<<( std::ostream& os, RawData const& data) {
	unsigned int nCoors = data.getNcoors();
	unsigned int slotSize = data.getStride() * 3;
	double* coors = data.getCoors();
	
	for (unsigned int i=0; i<nCoors; i++) {
		switch (data.getType()) {
			case VERTEX:
				os << coors[i * slotSize];
				os << " " << coors[i * slotSize + 1];
				os << " " << coors[i * slotSize + 2] << std::endl;
				break;
			case VERTEX_TEXT:
				os << coors[i * slotSize];
				os << " " << coors[i * slotSize + 1];
				os << " " << coors[i * slotSize + 2];
				os << " " << coors[i * slotSize + 3];
				os << " " << coors[i * slotSize + 3 + 1];
				os << " " << coors[i * slotSize + 3 + 2] << std::endl;
				break;
			case VERTEX_VNORM:
				os << coors[i * slotSize];
				os << " " << coors[i * slotSize + 1];
				os << " " << coors[i * slotSize + 2];
				os << " " << coors[i * slotSize + 6];
				os << " " << coors[i * slotSize + 6 + 1];
				os << " " << coors[i * slotSize + 6 + 2] << std::endl;
				break;
			case VERTEX_TEXT_VNORM:
				os << coors[i * slotSize];
				os << " " << coors[i * slotSize + 1];
				os << " " << coors[i * slotSize + 2];
				os << " " << coors[i * slotSize + 3];
				os << " " << coors[i * slotSize + 3 + 1];
				os << " " << coors[i * slotSize + 3 + 2];
				os << " " << coors[i * slotSize + 6];
				os << " " << coors[i * slotSize + 6 + 1];
				os << " " << coors[i * slotSize + 6 + 2] << std::endl;
				break;
		}
	}

	slotSize = 3;
	unsigned int *pos = data.getIndex(VERTEX);
	for (unsigned int i=0; i<data.getNindex(VERTEX); i++) {
		os << pos[i * slotSize];
		os << " " << pos[i * slotSize + 1];
		os << " " << pos[i * slotSize + 2] << std::endl;
	}
	slotSize = 6;
	pos = data.getIndex(VERTEX_TEXT);
	for (unsigned int i=0; i<data.getNindex(VERTEX_TEXT); i++) {
		os << pos[i * slotSize] << "/" << pos[i * slotSize + 1];
		os << " " << pos[i * slotSize + 2]  << "/" << pos[i * slotSize + 3];
		os << " " << pos[i * slotSize + 4] << "/" << pos[i * slotSize + 5] << std::endl;
	}
	pos = data.getIndex(VERTEX_VNORM);
	for (unsigned int i=0; i<data.getNindex(VERTEX_VNORM); i++) {
		os << pos[i * slotSize] << "//" << pos[i * slotSize + 1];
		os << " " << pos[i * slotSize + 2] << "//" << pos[i * slotSize + 3];
		os << " " << pos[i * slotSize + 4] << "//" << pos[i * slotSize + 5] << std::endl;
	}
	slotSize = 9;
	pos = data.getIndex(VERTEX_TEXT_VNORM);
	for (unsigned int i=0; i<data.getNindex(VERTEX_TEXT_VNORM); i++) {
		os << pos[i * slotSize] << "/" << pos[i * slotSize + 1] << "/" << pos[i * slotSize + 2];
		os << " " << pos[i * slotSize + 3] << "/" << pos[i * slotSize + 4] << "/" << pos[i * slotSize + 5];
		os << " " << pos[i * slotSize + 6] << "/" << pos[i * slotSize + 7] << "/" << pos[i * slotSize + 8] << std::endl;
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
	for (Face const& face : obj.getFaces())
		os << "f: " << face << std::endl;
	for (Line const& line : obj.getLines())
		os << "l: " << line << std::endl;
	return os;
}
