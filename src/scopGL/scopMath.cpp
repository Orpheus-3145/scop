#include "scopGL/scopMath.hpp"


Matrix4::Matrix4( float value ) noexcept {
	for(int i=0; i< 16; i++)
		this->_data[i] = value;
}

Matrix4::Matrix4( std::array<float,4> const& x, std::array<float,4> const& y, std::array<float,4> const& z, std::array<float,4> const& w ) noexcept {
	for (int i=0; i<4; i++) {
		this->_data[4 * i] = x[i];
		this->_data[4 * i + 1] = y[i];
		this->_data[4 * i + 2] = z[i];
		this->_data[4 * i + 3] = w[i];
	}
}

Matrix4::Matrix4( Matrix4 const& other ) noexcept {
	this->_data = other._data;
}

Matrix4::Matrix4( Matrix4&& other ) noexcept {
	this->_data = other._data;
}

Matrix4& Matrix4::operator=( Matrix4 const& other ) noexcept {
	if (this != &other)
		this->_data = other._data;
	return *this;
}

Matrix4& Matrix4::operator=( Matrix4&& other ) noexcept {
	if (this != &other)
		this->_data = other._data;
	return *this;
}

float& Matrix4::at( unsigned int row, unsigned int col ) {
	if (col * 4 + row > 15)
		throw MathException("Index out of bounds");
	return this->_data[col * 4 + row];
}

float const& Matrix4::at( unsigned int row, unsigned int col ) const {
	if (col * 4 + row > 15)
		throw MathException("Index out of bounds");
	return this->_data[col * 4 + row];
}

float const* Matrix4::data( void ) const noexcept{
	return this->_data.data();
}

Matrix4 Matrix4::operator+( Matrix4 const& other ) const noexcept {
	std::array<float,16> sum;

	for (int i=0; i<16; i++)
		sum[i] = this->_data[i] + other._data[i];
	return Matrix4(sum);
}

void Matrix4::operator+=( Matrix4 const& other ) noexcept {
	for (int i=0; i<16; i++)
		this->_data[i] += other._data[i];
}

Matrix4 Matrix4::operator*( Matrix4 const& other ) const noexcept {
	Matrix4 matProd;

	for (int row=0; row<4; row++) {
		for (int col=0; col<4; col++)
			matProd.at(row, col) = 
				this->at(row, 0) * other.at(0, col) + 
				this->at(row, 1) * other.at(1, col) + 
				this->at(row, 2) * other.at(2, col) + 
				this->at(row, 3) * other.at(3, col);
	}
	return matProd;
}

void Matrix4::operator*=( Matrix4 const& other ) noexcept {
	for (int row=0; row<4; row++) {
		for (int col=0; col<4; col++)
			this->at(row, col) = 
				this->at(row, 0) * other.at(0, col) + 
				this->at(row, 1) * other.at(1, col) + 
				this->at(row, 2) * other.at(2, col) + 
				this->at(row, 3) * other.at(3, col);
	}
}

Matrix4 createIdMat( void ) {
	return Matrix4({
		1.0f,  .0f,  .0f,  .0f,
		 .0f, 1.0f,  .0f,  .0f,
		 .0f,  .0f, 1.0f,  .0f,
		 .0f,  .0f,  .0f, 1.0f
	});
}

Matrix4 createTransMat( std::array<float,3> transArray ) {
	return Matrix4({
		1.0f,           .0f,           .0f,           .0f,
		 .0f,          1.0f,           .0f,           .0f,
		 .0f,           .0f,          1.0f,           .0f,
		transArray[0], transArray[1], transArray[2], 1.0f
	});
}

Matrix4 createTransMat( float translation ) {
	return Matrix4({
		1.0f,         .0f,         .0f,         .0f,
		 .0f,        1.0f,         .0f,         .0f,
		 .0f,         .0f,        1.0f,         .0f,
		translation, translation, translation, 1.0f
	});
}

Matrix4 createScaleMat( std::array<float,3> scaleArray ) {
	return Matrix4({
		scaleArray[0], .0f,           .0f,           .0f,
		.0f,           scaleArray[1], .0f,           .0f,
		.0f,           .0f,           scaleArray[2], .0f,
		.0f,           .0f,           .0f,           1.0f
	});
}

Matrix4 createScaleMat( float scale ) {
	return Matrix4({
		scale, .0f,   .0f,   .0f,
		.0f,   scale, .0f,   .0f,
		.0f,   .0f,   scale, .0f,
		.0f,   .0f,   .0f,   1.0f
	});
}

Matrix4 createRotationMat( float tetha, std::array<float,3> rotAxis ) {
	return Matrix4({
		cosf(tetha) + powf(rotAxis[0], 2) * (1 - cosf(tetha)),                    rotAxis[0] * rotAxis[1] * (1 - cosf(tetha)) + rotAxis[2] * sinf(tetha),  rotAxis[0] * rotAxis[2] * (1 - cosf(tetha)) - rotAxis[1] * sinf(tetha),  .0f,
		rotAxis[0] * rotAxis[1] * (1 - cosf(tetha)) - rotAxis[2] * sinf(tetha),  cosf(tetha) + powf(rotAxis[1], 2) * (1 - cosf(tetha)),                    rotAxis[1] * rotAxis[2] * (1 - cosf(tetha)) + rotAxis[0] * sinf(tetha),  .0f,
		rotAxis[0] * rotAxis[2] * (1 - cosf(tetha)) + rotAxis[1] * sinf(tetha),  rotAxis[1] * rotAxis[2] * (1 - cosf(tetha)) - rotAxis[0] * sinf(tetha),  cosf(tetha) + powf(rotAxis[2], 2) * (1 - cosf(tetha)),                    .0f,
		.0f,                                                                     .0f,                                                                     .0f,                                                                     1.0f
	});
}

float toRadiants( float angle ) {
	return angle * M_PI / 180.f;
}

float toDegrees( float radiants ) {
	return radiants * 180.f / M_PI;
}
