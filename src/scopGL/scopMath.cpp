#include "scopGL/scopMath.hpp"


Matrix4::Matrix4( float value ) noexcept {
	for(uint32_t i=0; i< 16; i++)
		this->_data[i] = value;
}

Matrix4::Matrix4( std::array<float,4> const& x, std::array<float,4> const& y, std::array<float,4> const& z, std::array<float,4> const& w ) noexcept {
	for (uint32_t i=0; i<4; i++) {
		this->_data[4 * i] = x[i];
		this->_data[4 * i + 1] = y[i];
		this->_data[4 * i + 2] = z[i];
		this->_data[4 * i + 3] = w[i];
	}
}

float& Matrix4::at( uint32_t row, uint32_t col ) {
	if (col * 4 + row > 15)
		throw MathException("Index out of bounds");
	return this->_data[col * 4 + row];
}

float const& Matrix4::at( uint32_t row, uint32_t col ) const {
	if (col * 4 + row > 15)
		throw MathException("Index out of bounds");
	return this->_data[col * 4 + row];
}

float const* Matrix4::data( void ) const noexcept{
	return this->_data.data();
}

Matrix4 Matrix4::operator+( Matrix4 const& other ) const noexcept {
	std::array<float,16> sum;

	for (uint32_t i=0; i<16; i++)
		sum[i] = this->_data[i] + other._data[i];
	return Matrix4(sum);
}

void Matrix4::operator+=( Matrix4 const& other ) noexcept {
	for (uint32_t i=0; i<16; i++)
		this->_data[i] += other._data[i];
}

Matrix4 Matrix4::operator*( Matrix4 const& other ) const noexcept {
	Matrix4 matProd;

	for (uint32_t row=0; row<4; row++) {
		for (uint32_t col=0; col<4; col++)
			matProd.at(row, col) = 
				this->at(row, 0) * other.at(0, col) + 
				this->at(row, 1) * other.at(1, col) + 
				this->at(row, 2) * other.at(2, col) + 
				this->at(row, 3) * other.at(3, col);
	}
	return matProd;
}

void Matrix4::operator*=( Matrix4 const& other ) noexcept {
	for (uint32_t row=0; row<4; row++) {
		for (uint32_t col=0; col<4; col++)
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

Matrix4 transMat( std::array<float,3> transArray ) {
	return Matrix4({
		1.0f,           .0f,           .0f,           .0f,
		 .0f,          1.0f,           .0f,           .0f,
		 .0f,           .0f,          1.0f,           .0f,
		transArray[0], transArray[1], transArray[2], 1.0f
	});
}

Matrix4 transMat( float translation ) {
	return Matrix4({
		1.0f,         .0f,         .0f,         .0f,
		 .0f,        1.0f,         .0f,         .0f,
		 .0f,         .0f,        1.0f,         .0f,
		translation, translation, translation, 1.0f
	});
}

Matrix4 scaleMat( std::array<float,3> scaleArray ) {
	return Matrix4({
		scaleArray[0], .0f,           .0f,           .0f,
		.0f,           scaleArray[1], .0f,           .0f,
		.0f,           .0f,           scaleArray[2], .0f,
		.0f,           .0f,           .0f,           1.0f
	});
}

Matrix4 scaleMat( float scale ) {
	return Matrix4({
		scale, .0f,   .0f,   .0f,
		.0f,   scale, .0f,   .0f,
		.0f,   .0f,   scale, .0f,
		.0f,   .0f,   .0f,   1.0f
	});
}

Matrix4 rotationMat( float tetha, std::array<float,3> rotAxis ) {
	float x = rotAxis[0];
	float y = rotAxis[1];
	float z = rotAxis[2];
	float sin = sinf(tetha);
	float cos = cosf(tetha);

	return Matrix4({
		cos + powf(x, 2) * (1 - cos),  x * y * (1 - cos) + z * sin,   x * z * (1 - cos) - y * sin,   .0f,
		x * y * (1 - cos) - z * sin,   cos + powf(y, 2) * (1 - cos),  y * z * (1 - cos) + x * sin,   .0f,
		x * z * (1 - cos) + y * sin,   y * z * (1 - cos) - x * sin,   cos + powf(z, 2) * (1 - cos),  .0f,
		.0f,                           .0f,                           .0f,                           1.0f
	});
}

Matrix4	projectionMatFinite( float fov, float aspect, float near, float far ) {
	if ((fov < -M_PI * 2) or (fov > M_PI * 2))
		fov = toRadiants(fov);
	float f = 1.0f / tanf(fov / 2.0f);

	return Matrix4({
		f / aspect,  .0f,  .0f,                               .0f,
		.0f,         f,    .0f,                               .0f,
		.0f,         .0f,  -1 * (far + near) / (far - near),  -1.0f,
		.0f,         .0f,  -2 * far * near / (far  - near),   .0f
	});
}

Matrix4 projectionMatInfinite( float fov, float aspect, float near) {
	if ((fov < -M_PI * 2) or (fov > M_PI * 2))
		fov = toRadiants(fov);
	float f = 1.0f / tanf(fov / 2.0f);

	return Matrix4({
		f / aspect,  .0f,  .0f,       .0f,
		.0f,         f,    .0f,       .0f,
		.0f,         .0f,  -1,        -1.0f,
		.0f,         .0f,  -2 * near,  .0f
	});
}

float toRadiants( float angle ) {
	return angle * M_PI / 180.f;
}

float toDegrees( float radiants ) {
	return radiants * 180.f / M_PI;
}
