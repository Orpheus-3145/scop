#include "parser/parserTypes.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <string>

coor2D::coor2D( coor2D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
}

coor2D::coor2D( std::vector<double> const& coors) {
	if (coors.size() != 2)
		throw ParsingException("Invalid size for vector to initialize a coor2D instance, has to be 2");
	this->_x = coors[0];
	this->_y = coors[1];
}

coor2D& coor2D::operator=(coor2D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	return (*this);
}


coor3D::coor3D( coor3D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	this->_z = coor._z;
}

coor3D::coor3D( std::vector<double> const& coors) {
	if (coors.size() != 3)
		throw ParsingException("Invalid size for vector to initialize a coor3D instance, has to be 3");
	this->_x = coors[0];
	this->_y = coors[1];
	this->_z = coors[2];
}

coor3D& coor3D::operator=(coor3D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	this->_z = coor._z;
	return (*this);
}


coor4D::coor4D( coor4D const& coor) noexcept {
	this->_x = coor._x;
	this->_y = coor._y;
	this->_z = coor._z;
	this->_w = coor._w;
}

coor4D::coor4D( std::vector<double> const& coors) {
	if (coors.size() != 4)
		throw ParsingException("Invalid size for vector to initialize a coor4D instance, has to be 4");
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
	return (*this);
}


index2D::index2D( index2D const& coor) noexcept {
	this->_i1 = coor._i1;
	this->_i2 = coor._i2;
}

index2D& index2D::operator=(index2D const& coor) noexcept {
	this->_i1 = coor._i1;
	this->_i2 = coor._i2;
	return (*this);
}


index3D::index3D( index3D const& coor) noexcept {
	this->_i1 = coor._i1;
	this->_i2 = coor._i2;
	this->_i3 = coor._i3;
}

index3D& index3D::operator=(index3D const& coor) noexcept {
	this->_i1 = coor._i1;
	this->_i2 = coor._i2;
	this->_i3 = coor._i3;
	return (*this);
}


VertexCoor::VertexCoor( std::vector<double> const& coorVect ) {
	switch(coorVect.size()) {
		case 3:
			this->_vertex = t_coor3D(coorVect[0], coorVect[1], coorVect[2]);
			break;
		case 4:
			this->_vertex = t_coor4D(coorVect[0], coorVect[1], coorVect[2], coorVect[3]);;
			break;
		default:
			throw ParsingException("Invalid size for vector to initialize a VertexCoor instance, has to be 3 or 4");
	}
}


TextureCoor::TextureCoor( std::vector<double> const& coorVect ) {
	switch(coorVect.size()) {
		case 1:
			this->_vertex = coorVect[0];
			break;
		case 2:
			this->_vertex = t_coor2D(coorVect[0], coorVect[1]);
			break;
		case 3:
			this->_vertex = t_coor3D(coorVect[0], coorVect[1], coorVect[2]);;
			break;
		default:
			throw ParsingException("Invalid size for vector to initialize a TextureCoor instance, has to be 1, 2 or 3");
	}
}


VertexSpaceParamCoor::VertexSpaceParamCoor( std::vector<double> const& coorVect ) {
	switch(coorVect.size()) {
		case 1:
			this->_vertex = coorVect[0];
			break;
		case 2:
			this->_vertex = t_coor2D(coorVect[0], coorVect[1]);
			break;
		case 3:
			this->_vertex = t_coor3D(coorVect[0], coorVect[1], coorVect[2]);;
			break;
		default:
			throw ParsingException("Invalid size for vector to initialize a VertexSpaceParamCoor instance, has to be 1, 2 or 3");
	}
}


FaceCoor::FaceCoor( std::vector<unsigned int> const& indexVect ) {
	switch(indexVect.size()) {
		case 1:
			this->index = indexVect[0];
			break;
		case 2:
			this->index = t_index2D(indexVect[0], indexVect[1]);
			break;
		case 3:
			this->index = t_index3D(indexVect[0], indexVect[1], indexVect[2]);;
			break;
		default:
			throw ParsingException("Invalid size for vector to initialize a FaceCoor instance, has to be 1, 2 or 3");
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

void Face::setSmoothing( int newSmoothing ) noexcept {
	this->_smoothing = newSmoothing;
}

FaceType Face::getFaceType( void ) const noexcept {
	return this->_type;
}

std::array<FaceCoor, 3> Face::getCoors( void ) const noexcept {
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


void ObjData::addTmlFile( std::string const& newFile ) noexcept {
	this->_tmlFiles.push_back(newFile);
}

void ObjData::addVertex( VertexCoor const& newVertex ) noexcept {
	this->_vertices.push_back(newVertex);
}

void ObjData::addTextureCoor( TextureCoor const& newTexture ) noexcept {
	this->_textureCoors.push_back(newTexture);
}

void ObjData::addVertexNorm( VertexNormCoor const& newVertexNorm ) noexcept {
	this->_verticesNorm.push_back(newVertexNorm);
}

void ObjData::addParamSpaceVertex( VertexSpaceParamCoor const& newVertexParam ) noexcept {
	this->_paramSpaceVertices.push_back(newVertexParam);
}

void ObjData::addFace( Face const& newFace ) noexcept {
	this->_faces.push_back(newFace);
}

void ObjData::addLine( Line const& newLine ) noexcept {
	this->_lines.push_back(newLine);
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
