#include "scopGL/scopMath.hpp"


VectF2 VectF2::from_array( std::array<float,2> const& coor) noexcept {
	return VectF2{coor[0], coor[1]};
}

std::array<float,2> VectF2::to_array( VectF2 const& v ) noexcept {
	return std::array<float,2>({v.x, v.y});
}

VectF3 VectF3::from_array( std::array<float,3> const& coor) noexcept {
	return VectF3{coor[0], coor[1], coor[2]};
}

VectF3 VectF3::from_vector( std::vector<float> const& coor ) {
	if (coor.size() == 0)
		throw MathException("Vector is empty, can't instantiate VectF3");
	else if (coor.size() == 1)
		return VectF3{coor[0], 0.0f, 0.0f};
	else if (coor.size() == 2)
		return VectF3{coor[0], coor[1], 0.0f};
	else
		return VectF3{coor[0], coor[1], coor[2]};
}

std::array<float,3> VectF3::to_array( VectF3 const& v ) noexcept {
	return std::array<float,3>({v.x, v.y, v.z});
}

VectUI3 VectUI3::from_array( std::array<uint32_t,3> const& positions) noexcept {
	return VectUI3{positions[0], positions[1], positions[2]};
}

VectUI3 VectUI3::from_vector( std::vector<uint32_t> const& coor ) {
	if (coor.size() == 0)
		throw MathException("Vector is empty, can't instantiate VectUI3");
	else if (coor.size() == 1)
		return VectUI3{coor[0], 0U, 0U};
	else if (coor.size() == 2)
		return VectUI3{coor[0], coor[1], 0U};
	else
		return VectUI3{coor[0], coor[1], coor[2]};
}

std::array<uint32_t,3> VectUI3::to_array( VectUI3 const& v ) noexcept {
	return std::array<uint32_t,3>({v.i1, v.i2, v.i3});
}

bool operator==( VectF2 const& v1, VectF2 const& v2 ) {
	return v1.x == v2.x and v1.y == v2.y;
}

bool operator==( VectF3 const& v1, VectF3 const& v2 ) {
	return v1.x == v2.x and v1.y == v2.y and v1.z == v2.z;
}

bool operator!=( VectF2 const& v1, VectF2 const& v2 ) {
	return !(v1 == v2);
}

bool operator!=( VectF3 const& v1, VectF3 const& v2 ) {
	return !(v1 == v2);
}

VectF2	operator+( VectF2 const& v1, VectF2 const& v2 ) {
	return VectF2{v1.x + v2.x, v1.y + v2.y};
}

VectF2&	operator+=( VectF2& v1, VectF2 const& v2 ) {
	v1.x += v2.x;
	v1.y += v2.y;
	return v1;
}

VectF2	operator-( VectF2 const& v1, VectF2 const& v2 ) {
	return VectF2{v1.x - v2.x, v1.y - v2.y};
}

VectF2&	operator-=( VectF2& v1, VectF2 const& v2 ) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	return v1;
}

VectF3	operator+( VectF3 const& v1, VectF3 const& v2 ) {
	return VectF3{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

VectF3&	operator+=( VectF3& v1, VectF3 const& v2 ) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

VectF3	operator-( VectF3 const& v1, VectF3 const& v2 ) {
	return VectF3{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

VectF3&	operator-=( VectF3& v1, VectF3 const& v2 ) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}

VectF2	operator+( VectF2 const& vector, float scalar ) {
	return VectF2{vector.x + scalar, vector.y + scalar};
}

VectF2	operator+( float scalar, VectF2 const& vector ) {
	return vector + scalar;
}

VectF2&	operator+=( VectF2& vector, float scalar ) {
	vector.x += scalar;
	vector.y += scalar;
	return vector;
}

VectF2	operator-( VectF2 const& vector, float scalar ) {
	return VectF2{vector.x - scalar, vector.y - scalar};
}

VectF2	operator-( float scalar, VectF2 const& vector ) {
	return vector * -1 + scalar;
}

VectF2&	operator-=( VectF2& vector, float scalar ) {
	vector.x -= scalar;
	vector.y -= scalar;
	return vector;
}

VectF2	operator*( VectF2 const& vector, float scalar ) {
	return VectF2{vector.x * scalar, vector.y * scalar};
}

VectF2	operator*( float scalar, VectF2 const& vector ) {
	return vector * scalar;
}

VectF2&	operator*=( VectF2& vector, float scalar ) {
	vector.x *= scalar;
	vector.y *= scalar;
	return vector;
}

VectF2	operator/( VectF2 const& vector, float scalar ) {
	if (std::fabs(scalar) < F_ZERO)
		throw MathException("Zero vector-scalar division");
	return VectF2{vector.x / scalar, vector.y / scalar};
}

VectF2&	operator/=( VectF2& vector, float scalar ) {
	vector.x /= scalar;
	vector.y /= scalar;
	return vector;
}

VectF3	operator+( VectF3 const& vector, float scalar ) {
	return VectF3{vector.x + scalar, vector.y + scalar, vector.z + scalar};
}

VectF3	operator+( float scalar, VectF3 const& vector ) {
	return vector + scalar;
}

VectF3&	operator+=( VectF3& vector, float scalar ) {
	vector.x += scalar;
	vector.y += scalar;
	vector.z += scalar;
	return vector;
}

VectF3	operator-( VectF3 const& vector, float scalar ) {
	return VectF3{vector.x - scalar, vector.y - scalar, vector.z - scalar};
}

VectF3	operator-( float scalar, VectF3 const& vector ) {
	return vector * -1 + scalar;
}

VectF3&	operator-=( VectF3& vector, float scalar ) {
	vector.x -= scalar;
	vector.y -= scalar;
	vector.z -= scalar;
	return vector;
}

VectF3	operator*( VectF3 const& vector, float scalar ) {
	return VectF3{vector.x * scalar, vector.y * scalar, vector.z * scalar};
}

VectF3	operator*( float scalar, VectF3 const& vector ) {
	return vector * scalar;
}

VectF3&	operator*=( VectF3& vector, float scalar ) {
	vector.x *= scalar;
	vector.y *= scalar;
	vector.z *= scalar;
	return vector;
}

VectF3	operator/( VectF3 const& vector, float scalar ) {
	if (std::fabs(scalar) < F_ZERO)
		throw MathException("Zero vector-scalar division");
	return VectF3{vector.x / scalar, vector.y / scalar, vector.z / scalar};
}

VectF3&	operator/=( VectF3& vector, float scalar ) {
	vector.x /= scalar;
	vector.y /= scalar;
	vector.z /= scalar;
	return vector;
}

float operator*( VectF2 const& v1, VectF2 const& v2 ) {
	return v1.x * v2.x + v1.y * v2.y;
}

float operator*( VectF3 const& v1, VectF3 const& v2 ) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float operator^( VectF2 const& v1, VectF2 const& v2 ) {
	return v1.x * v2.y - v1.y * v2.x;
}

VectF3 operator^( VectF3 const& v1, VectF3 const& v2 ) {
	return VectF3{
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

VectF3&	operator^=( VectF3& v1, VectF3 const& v2 ) {
	v1.x = v1.y * v2.z - v1.z * v2.y;
	v1.y = v1.z * v2.x - v1.x * v2.z;
	v1.z = v1.x * v2.y - v1.y * v2.x;
	return v1;
}

std::ostream& operator<<(std::ostream& os, VectF2 const& coor) {
	os << coor.x << " " << coor.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, VectF3 const& coor) {
	os << coor.x << " " << coor.y << " " << coor.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, VectUI3 const& index) {
	os << index.i1 << " " << index.i2 << " " << index.i3;
	return os;
}

float getAbs( VectF2 const& v ) {
	return sqrtf(powf(v.x, 2) + powf(v.y, 2));
}

float getAbs( VectF3 const& v ) {
	return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
}

VectF2 normalize(VectF2 const& v) {
	float lenght = getAbs(v);
	if (lenght < F_ZERO)
		return v;
	else
		return v / getAbs(v);
}

VectF3 normalize(VectF3 const& v) {
	float lenght = getAbs(v);
	if (lenght < F_ZERO)
		return v;
	else
		return v / getAbs(v);
}

VectF3 getNormal( VectF3 const& v1, VectF3 const& v2, VectF3 const& v3, bool normalized ) {
	VectF3 normal = (v2 - v1) ^ (v3 - v1);
	if (normalized)
		return normalize(normal);
	else
		return normal;
}

VectF3 getNormal( std::vector<VectF3> const& v, bool normalized ) {
	if (v.size() < 3)
		throw MathException("Vector doesn't have enough elements, needs 3");
	return getNormal(v[0], v[1], v[2], normalized);
}

VectF3 getNormal( std::array<VectF3,3> const& v, bool normalized ) {
	return getNormal(v[0], v[1], v[2], normalized);
}

bool isCCWorient( VectF3 const& v1, VectF3 const& v2, VectF3 const& v3, VectF3 const meshCenter ) {
	VectF3 normal = getNormal(v1, v2, v3);
	// center of the triangle
	VectF3 faceCenter = (v1 + v2 + v3) / 3.0f;
	VectF3 toOutside = faceCenter - meshCenter;
	return (normal * toOutside) < F_ZERO;
}

bool isCCWorient( std::vector<VectF3> const& vertexes, VectF3 const meshCenter ) {
	if (vertexes.size() < 3)
		throw MathException("Vector doesn't have enough elements, needs 3");
	return isCCWorient(vertexes[0], vertexes[1], vertexes[2], meshCenter);
}

bool isCCWorient( std::array<VectF3,3> const& vertexes, VectF3 const meshCenter ) {
	return isCCWorient(vertexes[0], vertexes[1], vertexes[2], meshCenter);
}

bool isCWorient( VectF3 const& v1, VectF3 const& v2, VectF3 const& v3, VectF3 const meshCenter ) {
	return !isCCWorient(v1, v2, v3, meshCenter);
}

bool isCWorient( std::vector<VectF3> const& vertexes, VectF3 const meshCenter ) {
	if (vertexes.size() < 3)
		throw MathException("Vector doesn't have enough elements, needs 3");
	return !isCCWorient(vertexes, meshCenter);
}

bool isCWorient( std::array<VectF3,3> const& vertexes, VectF3 const meshCenter ) {
	return !isCCWorient(vertexes, meshCenter);
}

float width( VectF2 const& pre, VectF2 const& center, VectF2 const& post ) {
	float dotProd = (pre - center) * (post - center);
	float lenPre = getAbs(pre - center);
	float lenPost = getAbs(post - center);
	// do clamping to normalize weird floats like 1.00000001 or -1.00000001 (where acosf would return NaN)
	float cosTetha = std::clamp(dotProd / (lenPre * lenPost), -1.0f, 1.0f);
	return acosf(cosTetha);
}

bool triangleContainmentTest( VectF2 const& v1, VectF2 const& v2, VectF2 const& v3, VectF2 const& check ) {
	bool b1 = ((check - v2) ^ (v1 - v2)) < F_ZERO;
	bool b2 = ((check - v3) ^ (v2 - v3)) < F_ZERO;
	bool b3 = ((check - v1) ^ (v3 - v1)) < F_ZERO;
	return (b1 == b2) and (b2 == b3); 
}



Matrix4 idMat( void ) {
	return Matrix4(std::array<float,16>{
		1.0f,  .0f,  .0f,  .0f,
		 .0f, 1.0f,  .0f,  .0f,
		 .0f,  .0f, 1.0f,  .0f,
		 .0f,  .0f,  .0f, 1.0f
	});
}

Matrix4 transMat( std::array<float,3> const& transArray, bool isColumnMajor ) {
	Matrix4 translation(std::array<float,16>{
		1.0f,  .0f,  .0f, transArray[0],
		 .0f, 1.0f,  .0f, transArray[1],
		 .0f,  .0f, 1.0f, transArray[2],
		 .0f,  .0f, 1.0f, 1.0f
	});
	if (isColumnMajor == true)
		translation.transpose();
	return translation;
}

Matrix4 transMat( VectF3 const& transVect, bool isColumnMajor ) {
	Matrix4 translation(std::array<float,16>{
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
	Matrix4 translation(std::array<float,16>{
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
	return Matrix4(std::array<float,16>{
		scaleArray[0], .0f,           .0f,           .0f,
		.0f,           scaleArray[1], .0f,           .0f,
		.0f,           .0f,           scaleArray[2], .0f,
		.0f,           .0f,           .0f,           1.0f
	});
}

Matrix4 scaleMat( VectF3 const& scaleVect ) {
	return Matrix4(std::array<float,16>{
		scaleVect.x, .0f,           .0f,           .0f,
		.0f,           scaleVect.y, .0f,           .0f,
		.0f,           .0f,           scaleVect.z, .0f,
		.0f,           .0f,           .0f,           1.0f
	});
}

Matrix4 scaleMat( float scale ) {
	return Matrix4(std::array<float,16>{
		scale, .0f,   .0f,   .0f,
		.0f,   scale, .0f,   .0f,
		.0f,   .0f,   scale, .0f,
		.0f,   .0f,   .0f,   1.0f
	});
}

Matrix4 rotationMat( float tetha, VectF3 const& rotAxis, bool isColumnMajor ) {
	float x = rotAxis.x;
	float y = rotAxis.y;
	float z = rotAxis.z;
	float sin = sinf(tetha);
	float cos = cosf(tetha);

	Matrix4 rotation(std::array<float,16>{
		cos + powf(x, 2) * (1 - cos),  x * y * (1 - cos) - z * sin,   x * z * (1 - cos) + y * sin,    .0f,
		x * y * (1 - cos) + z * sin,   cos + powf(y, 2) * (1 - cos),  y * z * (1 - cos) - x * sin,    .0f,
		x * z * (1 - cos) - y * sin,   y * z * (1 - cos) + x * sin,   cos + powf(z, 2) * (1 - cos),   .0f,
		 .0f,                           .0f,                           .0f,                          1.0f
	});
	if (isColumnMajor == true)
		rotation.transpose();
	return rotation;
}


Matrix4 projectionMatFinite( float fov, float aspect, float near, float far, bool isColumnMajor ) {
	if ((fov < -M_PI * 2) or (fov > M_PI * 2))
		fov = toRadiants(fov);
	float f = 1.0f / tanf(fov / 2.0f);
	Matrix4 projection(std::array<float,16>{
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

	Matrix4 projection(std::array<float,16>{
		f / aspect,  .0f,  .0f,        .0f,
		.0f,         f,    .0f,        .0f,
		.0f,         .0f,  -1,         -2 * near,
		.0f,         .0f,  -1.0f,       .0f
	});
	if (isColumnMajor == true)
		projection.transpose();
	return projection;
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
