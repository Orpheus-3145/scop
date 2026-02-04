#include <cmath>

#include "math/quaternion.hpp"
#include "math/utilities.hpp"
#include "exception.hpp"

// quternion that represents a rotation of an angle around a rotation axis
Quatern::Quatern( float angle, VectF3 const& rotAxis ) {
	this->w = cosf(angle);
	this->x = rotAxis.x * sinf(angle);
	this->y = rotAxis.y * sinf(angle);
	this->z = rotAxis.z * sinf(angle);
}

Quatern Quatern::conjugate( void ) const noexcept {
	return Quatern{this->w, -this->x, -this->y, -this->z};
}

Quatern Quatern::inverse( void ) const {
	if ((fabs(this->w) < F_ZERO) or (fabs(this->x) < F_ZERO) or (fabs(this->y) < F_ZERO) or (fabs(this->z) < F_ZERO))
		throw MathException("Quaternion is zero, inverse doesn't exists");
	float normPower2 = powf(this->w, 2.0f) + powf(this->x, 2.0f) + powf(this->y, 2.0f) + powf(this->z, 2.0f);
	return this->conjugate() / normPower2;
}

Matrix4 Quatern::getMatrix( void ) const noexcept {
	return Matrix4(std::array<float,16>{
		1 - 2 * powf(this->y, 2) - 2 * powf(this->z, 2),  2 * this->x * this->y - 2 * this->w * this->z,    2 * this->x * this->z + 2 * this->w * this->y,    0.0f,
		2 * this->x * this->y + 2 * this->w * this->z,    1 - 2 * powf(this->x, 2) - 2 * powf(this->z, 2),  2 * this->y * this->z - 2 * this->w * this->x,    0.0f,
		2 * this->x * this->z - 2 * this->w * this->y,    2 * this->y * this->z + 2 * this->w * this->x,    1 - 2 * powf(this->x, 2) - 2 * powf(this->y, 2),  0.0f,
		0.0f,                                             0.0f,                                             0.0f,                                             1.0f
	});
}

VectF3 Quatern::vector( void ) const noexcept {
	return VectF3{this->x, this->y, this->z};
}

Quatern& Quatern::operator+=( Quatern const& q ) noexcept {
	this->w += q.w;
	this->x += q.x;
	this->y += q.y;
	this->z += q.z;
	return *this;
}

Quatern& Quatern::operator-=( Quatern const& q ) noexcept {
	this->w -= q.w;
	this->x -= q.x;
	this->y -= q.y;
	this->z -= q.z;
	return *this;
}

Quatern& Quatern::operator*=( Quatern const& q ) noexcept {
	Quatern prod{
		this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z,
		this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y,
		this->w * q.y + this->y * q.w + this->z * q.x - this->x * q.z,
		this->w * q.z + this->z * q.w + this->x * q.y - this->y * q.x,
	};
	*this = prod;
	return *this;
}

bool operator==( Quatern const& q1, Quatern const& q2 ) {
	return (
		(fabs(q1.w - q2.w) < F_ZERO) and
		(fabs(q1.x - q2.x) < F_ZERO) and
		(fabs(q1.y - q2.y) < F_ZERO) and
		(fabs(q1.z - q2.z) < F_ZERO)
	);
}

bool operator!=( Quatern const& q1, Quatern const& q2 ) {
	return !(q1 == q2);
}

Quatern	operator+( Quatern const& q1, Quatern const& q2 ) {
	return Quatern{q1.w + q2.w, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z};
}

Quatern	operator-( Quatern const& q1, Quatern const& q2 ) {
	return Quatern{q1.w - q2.w, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z};
}

Quatern	operator*( Quatern const& q1, Quatern const& q2 ) {
	Quatern prod{
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
		q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
		q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
	};
	return prod;
}

Quatern	operator+( Quatern const& q, float real ) {
	return Quatern{q.w + real, q.x, q.y, q.z};
}

Quatern	operator+( float real, Quatern const& q ) {
	return q + real;
}

Quatern	operator-( Quatern const& q, float real ) {
	return Quatern{q.w - real, q.x, q.y, q.z};

}

Quatern	operator-( float real, Quatern const& q ) {
	return q * -1 + real;
}

Quatern	operator*( Quatern const& q1, float real ) {
	return Quatern(q1.w * real, q1.x * real, q1.y * real, q1.z * real);
}

Quatern	operator*( float real, Quatern const& q1 ) {
	return Quatern(q1.w * real, q1.x * real, q1.y * real, q1.z * real);
}

Quatern	operator/( Quatern const& q1, float real ) {
	if (fabs(real) < F_ZERO)
		throw MathException("Zero division");
	return Quatern(q1.w / real, q1.x / real, q1.y / real, q1.z / real);
}

std::ostream& operator<<( std::ostream& os, Quatern const& q ) {
	std::cout << q.w << " " << q.x << "i " << q.y << "j " << q.z << "k";
	return os;
}

float getAbs( Quatern const& q ) {
	return sqrtf(powf(q.w, 2.0f) + powf(q.x, 2.0f) + powf(q.y, 2.0f) + powf(q.z, 2.0f));
}
