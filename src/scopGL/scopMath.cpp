#include "scopGL/scopMath.hpp"


VectF2D VectF2D::from_array( std::array<float,2> const& coor) noexcept {
	return VectF2D{coor[0], coor[1]};
}

std::array<float,2> VectF2D::to_array( VectF2D const& v ) noexcept {
	return std::array<float,2>({v.x, v.y});
}

VectF3D VectF3D::from_array( std::array<float,3> const& coor) noexcept {
	return VectF3D{coor[0], coor[1], coor[2]};
}

VectF3D VectF3D::from_vector( std::vector<float> const& coor ) {
	if (coor.size() == 0)
		throw MathException("Vector is empty, can't instantiate VectF3D");
	else if (coor.size() == 1)
		return VectF3D{coor[0], 0.0f, 0.0f};
	else if (coor.size() == 2)
		return VectF3D{coor[0], coor[1], 0.0f};
	else
		return VectF3D{coor[0], coor[1], coor[2]};
}

std::array<float,3> VectF3D::to_array( VectF3D const& v ) noexcept {
	return std::array<float,3>({v.x, v.y, v.z});
}

VectUI3D VectUI3D::from_array( std::array<uint32_t,3> const& positions) noexcept {
	return VectUI3D{positions[0], positions[1], positions[2]};
}

VectUI3D VectUI3D::from_vector( std::vector<uint32_t> const& coor ) {
	if (coor.size() == 0)
		throw MathException("Vector is empty, can't instantiate VectUI3D");
	else if (coor.size() == 1)
		return VectUI3D{coor[0], 0U, 0U};
	else if (coor.size() == 2)
		return VectUI3D{coor[0], coor[1], 0U};
	else
		return VectUI3D{coor[0], coor[1], coor[2]};
}

std::array<uint32_t,3> VectUI3D::to_array( VectUI3D const& v ) noexcept {
	return std::array<uint32_t,3>({v.i1, v.i2, v.i3});
}

bool operator==( VectF2D const& v1, VectF2D const& v2 ) {
	return v1.x == v2.x and v1.y == v2.y;
}

bool operator==( VectF3D const& v1, VectF3D const& v2 ) {
	return v1.x == v2.x and v1.y == v2.y and v1.z == v2.z;
}

bool operator!=( VectF2D const& v1, VectF2D const& v2 ) {
	return !(v1 == v2);
}

bool operator!=( VectF3D const& v1, VectF3D const& v2 ) {
	return !(v1 == v2);
}

VectF2D	operator+( VectF2D const& v1, VectF2D const& v2 ) {
	return VectF2D{v1.x + v2.x, v1.y + v2.y};
}

VectF2D	operator-( VectF2D const& v1, VectF2D const& v2 ) {
	return VectF2D{v1.x - v2.x, v1.y - v2.y};
}

VectF3D	operator+( VectF3D const& v1, VectF3D const& v2 ) {
	return VectF3D{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

VectF3D	operator-( VectF3D const& v1, VectF3D const& v2 ) {
	return VectF3D{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

VectF2D	operator+( VectF2D const& vector, float scalar ) {
	return VectF2D{vector.x + scalar, vector.y + scalar};
}

VectF2D	operator+( float scalar, VectF2D const& vector ) {
	return vector + scalar;
}

VectF2D	operator-( VectF2D const& vector, float scalar ) {
	return VectF2D{vector.x - scalar, vector.y - scalar};
}

VectF2D	operator*( VectF2D const& vector, float scalar ) {
	return VectF2D{vector.x * scalar, vector.y * scalar};
}

VectF2D	operator*( float scalar, VectF2D const& vector ) {
	return vector * scalar;
}

VectF2D	operator/( VectF2D const& vector, float scalar ) {
	if (std::fabs(scalar) < F_ZERO)
		throw MathException("Zero vector-scalar division");
	return VectF2D{vector.x / scalar, vector.y / scalar};
}

VectF3D	operator+( VectF3D const& vector, float scalar ) {
	return VectF3D{vector.x + scalar, vector.y + scalar, vector.z + scalar};
}

VectF3D	operator+( float scalar, VectF3D const& vector ) {
	return vector + scalar;
}

VectF3D	operator-( VectF3D const& vector, float scalar ) {
	return VectF3D{vector.x - scalar, vector.y - scalar, vector.z - scalar};
}

VectF3D	operator*( VectF3D const& vector, float scalar ) {
	return VectF3D{vector.x * scalar, vector.y * scalar, vector.z * scalar};
}

VectF3D	operator*( float scalar, VectF3D const& vector ) {
	return vector * scalar;
}

VectF3D	operator/( VectF3D const& vector, float scalar ) {
	if (std::fabs(scalar) < F_ZERO)
		throw MathException("Zero vector-scalar division");
	return VectF3D{vector.x / scalar, vector.y / scalar, vector.z / scalar};
}

float operator*( VectF2D const& v1, VectF2D const& v2 ) {
	return v1.x * v2.x + v1.y * v2.y;
}

float operator*( VectF3D const& v1, VectF3D const& v2 ) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float operator^( VectF2D const& v1, VectF2D const& v2 ) {
	return v1.x * v2.y - v1.y * v2.x;
}

VectF3D operator^( VectF3D const& v1, VectF3D const& v2 ) {
	return VectF3D{
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

float getAbs( VectF2D const& v ) {
	return sqrtf(powf(v.x, 2) + powf(v.y, 2));
}

float getAbs( VectF3D const& v ) {
	return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
}

VectF2D normalize(VectF2D const& v) {
	float lenght = getAbs(v);
	if (lenght < F_ZERO)
		return v;
	else
		return v / getAbs(v);
}

VectF3D normalize(VectF3D const& v) {
	float lenght = getAbs(v);
	if (lenght < F_ZERO)
		return v;
	else
		return v / getAbs(v);
}

VectF3D	getNormal( VectF3D const& v1, VectF3D const& v2, VectF3D const& v3, bool normalized ) {
	VectF3D normal = (v2 - v1) ^ (v3 - v1);
	if (normalized)
		return normalize(normal);
	else
		return normal;
}

VectF3D	getNormal( std::vector<VectF3D> const& v, bool normalized ) {
	if (v.size() < 3)
		throw MathException("Vector doesn't have enough elements, needs 3");
	return getNormal(v[0], v[1], v[2], normalized);
}

VectF3D	getNormal( std::array<VectF3D,3> const& v, bool normalized ) {
	return getNormal(v[0], v[1], v[2], normalized);
}

bool isCCWorient( VectF3D const& v1, VectF3D const& v2, VectF3D const& v3, VectF3D const meshCenter ) {
	VectF3D normal = getNormal(v1, v2, v3);
	// center of the triangle
	VectF3D faceCenter = (v1 + v2 + v3) / 3.0f;
	VectF3D toOutside = faceCenter - meshCenter;
	return (normal * toOutside) < F_ZERO;
}

bool isCCWorient( std::vector<VectF3D> const& vertexes, VectF3D const meshCenter ) {
	if (vertexes.size() < 3)
		throw MathException("Vector doesn't have enough elements, needs 3");
	return isCCWorient(vertexes[0], vertexes[1], vertexes[2], meshCenter);
}

bool isCCWorient( std::array<VectF3D,3> const& vertexes, VectF3D const meshCenter ) {
	return isCCWorient(vertexes[0], vertexes[1], vertexes[2], meshCenter);
}

bool isCWorient( VectF3D const& v1, VectF3D const& v2, VectF3D const& v3, VectF3D const meshCenter ) {
	return !isCCWorient(v1, v2, v3, meshCenter);
}

bool isCWorient( std::vector<VectF3D> const& vertexes, VectF3D const meshCenter ) {
	if (vertexes.size() < 3)
		throw MathException("Vector doesn't have enough elements, needs 3");
	return !isCCWorient(vertexes, meshCenter);
}

bool isCWorient( std::array<VectF3D,3> const& vertexes, VectF3D const meshCenter ) {
	return !isCCWorient(vertexes, meshCenter);
}

float width( VectF2D const& pre, VectF2D const& center, VectF2D const& post ) {
	float dotProd = (pre - center) * (post - center);
	float lenPre = getAbs(pre - center);
	float lenPost = getAbs(post - center);
	// do clamping to normalize weird floats like 1.00000001 or -1.00000001 (where acosf would return NaN)
	float cosTetha = std::clamp(dotProd / (lenPre * lenPost), -1.0f, 1.0f);
	return acosf(cosTetha);
}

bool triangleContainmentTest( VectF2D const& v1, VectF2D const& v2, VectF2D const& v3, VectF2D const& check ) {
	bool b1 = ((check - v2) ^ (v1 - v2)) < F_ZERO;
	bool b2 = ((check - v3) ^ (v2 - v3)) < F_ZERO;
	bool b3 = ((check - v1) ^ (v3 - v1)) < F_ZERO;
	return (b1 == b2) and (b2 == b3); 
}

std::ostream& operator<<(std::ostream& os, VectF2D const& coor) {
	os << coor.x << " " << coor.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, VectF3D const& coor) {
	os << coor.x << " " << coor.y << " " << coor.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, VectUI3D const& index) {
	os << index.i1 << " " << index.i2 << " " << index.i3;
	return os;
}


Matrix4::Matrix4( float value ) noexcept {
	for(uint32_t i = 0; i < this->_data.size(); i++)
		this->_data[i] = value;
}

Matrix4::Matrix4( std::array<float,4> const& x, std::array<float,4> const& y, std::array<float,4> const& z, std::array<float,4> const& w ) noexcept {
	for (uint32_t i = 0; i < 4; i++) {
		this->_data[4 * i] = x[i];
		this->_data[4 * i + 1] = y[i];
		this->_data[4 * i + 2] = z[i];
		this->_data[4 * i + 3] = w[i];
	}
}

float& Matrix4::at( uint32_t row, uint32_t col ) {
	if ((row * 4 + col) > 15)
		throw MathException("Index out of bounds");
	return this->_data[row * 4 + col];
}

float const& Matrix4::at( uint32_t row, uint32_t col ) const {
	if (row * 4 + col > 15)
		throw MathException("Index out of bounds");
	return this->_data[row * 4 + col];
}

float const* Matrix4::data( void ) const noexcept{
	return this->_data.data();
}

void Matrix4::transpose( void) noexcept {
	for (uint32_t row=0; row<4; row++) {
		for (uint32_t col=row+1U; col<4; col++) {
			if (row != col)
				std::swap(this->at(row, col), this->at(col, row));
		}
	}
}

void Matrix4::operator+=( Matrix4 const& other ) noexcept {
	for (uint32_t i=0; i<16; i++)
		this->_data[i] += other._data[i];
}

void Matrix4::operator*=( Matrix4 const& other ) noexcept {
	for (uint32_t row=0; row<4; row++) {
		for (uint32_t col=0; col<4; col++)
			this->at(row, col) = 
				other.at(row, 0) * this->at(0, col) + 
				other.at(row, 1) * this->at(1, col) + 
				other.at(row, 2) * this->at(2, col) + 
				other.at(row, 3) * this->at(3, col);
	}
}


Matrix4 idMat( void ) {
	return Matrix4({
		1.0f,  .0f,  .0f,  .0f,
		 .0f, 1.0f,  .0f,  .0f,
		 .0f,  .0f, 1.0f,  .0f,
		 .0f,  .0f,  .0f, 1.0f
	});
}

Matrix4 transMat( std::array<float,3> const& transArray, bool isColumnMajor ) {
	Matrix4 translation({
		1.0f,  .0f,  .0f, transArray[0],
		 .0f, 1.0f,  .0f, transArray[1],
		 .0f,  .0f, 1.0f, transArray[2],
		 .0f,  .0f, 1.0f, 1.0f
	});
	if (isColumnMajor == true)
		translation.transpose();
	return translation;
}

Matrix4 transMat( VectF3D const& transVect, bool isColumnMajor ) {
	Matrix4 translation({
		1.0f,  .0f,  .0f, transVect.x,
		 .0f, 1.0f,  .0f, transVect.y,
		 .0f,  .0f, 1.0f, transVect.z,
		 .0f,  .0f, 1.0f, 1.0f
	});
	if (isColumnMajor == true)
		translation.transpose();
	return translation;
}

Matrix4 transMat( float uniTranslation, bool isColumnMajor ) {
	Matrix4 translation({
		1.0f,  .0f,  .0f, uniTranslation,
		 .0f, 1.0f,  .0f, uniTranslation,
		 .0f,  .0f, 1.0f, uniTranslation,
		 .0f,  .0f, 1.0f, 1.0f
	});
	if (isColumnMajor == true)
		translation.transpose();
	return translation;
}

Matrix4 scaleMat( std::array<float,3> const& scaleArray ) {
	return Matrix4({
		scaleArray[0], .0f,           .0f,           .0f,
		.0f,           scaleArray[1], .0f,           .0f,
		.0f,           .0f,           scaleArray[2], .0f,
		.0f,           .0f,           .0f,           1.0f
	});
}

Matrix4 scaleMat( VectF3D const& scaleVect ) {
	return Matrix4({
		scaleVect.x, .0f,           .0f,           .0f,
		.0f,           scaleVect.y, .0f,           .0f,
		.0f,           .0f,           scaleVect.z, .0f,
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

Matrix4 rotationMat( float tetha, std::array<float,3> rotAxis, bool isColumnMajor ) {
	float x = rotAxis[0];
	float y = rotAxis[1];
	float z = rotAxis[2];
	float sin = sinf(tetha);
	float cos = cosf(tetha);

	Matrix4 rotation({
		cos + powf(x, 2) * (1 - cos),  x * y * (1 - cos) - z * sin,   x * z * (1 - cos) + y * sin,    .0f,
		x * y * (1 - cos) + z * sin,   cos + powf(y, 2) * (1 - cos),  y * z * (1 - cos) - x * sin,    .0f,
		x * z * (1 - cos) - y * sin,   y * z * (1 - cos) + x * sin,   cos + powf(z, 2) * (1 - cos),   .0f,
		 .0f,                           .0f,                           .0f,                          1.0f
	});
	if (isColumnMajor == true)
		rotation.transpose();
	return rotation;
}

Matrix4 lookAt( VectF3D const& cameraPos, VectF3D const& cameraTarget, VectF3D const& up, bool isColumnMajor ) {
	VectF3D cameraDirection = normalize(cameraPos - cameraTarget);
	VectF3D cameraRight = normalize(up ^ cameraDirection);
	VectF3D cameraUp = cameraDirection ^ cameraRight;
	Matrix4 rotation{
		std::array<float,4>{cameraRight.x, cameraUp.x, cameraDirection.x, 0.0f},
		std::array<float,4>{cameraRight.y, cameraUp.y, cameraDirection.y, 0.0f},
		std::array<float,4>{cameraRight.z, cameraUp.z, cameraDirection.z, 0.0f},
		std::array<float,4>{0.0f, 0.0f, 0.0f, 1.0f}
	};
	Matrix4 translation = transMat(cameraPos * -1, false);
	// std::cout << "rot\n" << rotation << std::endl;
	// std::cout << "trans\n" << translation << std::endl;
	Matrix4 look = rotation * translation;
	if (isColumnMajor == true)
		look.transpose();
	return look;
}

Matrix4	projectionMatFinite( float fov, float aspect, float near, float far, bool isColumnMajor ) {
	if ((fov < -M_PI * 2) or (fov > M_PI * 2))
		fov = toRadiants(fov);
	float f = 1.0f / tanf(fov / 2.0f);
	Matrix4 projection({
		f / aspect,  .0f,  .0f,                               .0f,
		.0f,         f,    .0f,                               .0f,
		.0f,         .0f,  -1 * (far + near) / (far - near),  -2 * far * near / (far - near),
		.0f,         .0f,  -1.0f,                              .0f
	});
	if (isColumnMajor == true)
		projection.transpose();
	return projection;
}

Matrix4 projectionMatInfinite( float fov, float aspect, float near, bool isColumnMajor ) {
	if ((fov < -M_PI * 2) or (fov > M_PI * 2))
		fov = toRadiants(fov);
	float f = 1.0f / tanf(fov / 2.0f);

	Matrix4 projection({
		f / aspect,  .0f,  .0f,        .0f,
		.0f,         f,    .0f,        .0f,
		.0f,         .0f,  -1,         -2 * near,
		.0f,         .0f,  -1.0f,       .0f
	});
	if (isColumnMajor == true)
		projection.transpose();
	return projection;
}

Matrix4 operator+( Matrix4 const& m1, Matrix4 const& m2 ) {
	std::array<float,16> sum;

	for (uint32_t i=0; i<16; i++)
		sum[i] = m1._data[i] + m2._data[i];
	return Matrix4(sum);
}

Matrix4 operator*( Matrix4 const& m1, Matrix4 const& m2 ) {
	Matrix4 matProd;

	for (uint32_t row=0; row<4; row++) {
		for (uint32_t col=0; col<4; col++)
			matProd.at(row, col) = 
				m1.at(row, 0) * m2.at(0, col) + 
				m1.at(row, 1) * m2.at(1, col) + 
				m1.at(row, 2) * m2.at(2, col) + 
				m1.at(row, 3) * m2.at(3, col);
	}
	return matProd;
}

std::ostream& operator<<( std::ostream& os, Matrix4 const& mat) {
	for (uint32_t row=0; row<4; row++) {
		for (uint32_t col=0; col<4; col++)
			os << mat.at(row, col) << " ";
		os << std::endl;
	}
	return os;
}


float toRadiants( float angle ) {
	return ((static_cast<int32_t>(angle) % 360) * M_PI / 180.f);
}

float toDegrees( float radiants ) {
	return radiants * 180.f / M_PI;
}

float randomFloat( void ) {
	static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}
