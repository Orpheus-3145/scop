#pragma once
#include <array>
#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include <cstdint>

#include "exceptions.hpp"

static const float F_ZERO = 1e-6f;

struct VectF2 {
	float x;
	float y;

	static VectF2				from_array( std::array<float,2> const& ) noexcept;
	static std::array<float,2>	to_array( VectF2 const& ) noexcept;
};

struct VectF3 {
	float x;
	float y;
	float z;

	static VectF3				from_array( std::array<float,3> const& ) noexcept;
	static VectF3				from_vector( std::vector<float> const& );
	static std::array<float,3>	to_array( VectF3 const& ) noexcept;
};

struct VectUI3 {
	uint32_t i1;
	uint32_t i2;
	uint32_t i3;

	static VectUI3 					from_array( std::array<uint32_t,3> const& ) noexcept;
	static VectUI3					from_vector( std::vector<uint32_t> const& );
	static std::array<uint32_t,3>	to_array( VectUI3 const& ) noexcept;
};

bool	operator==( VectF2 const&, VectF2 const& );
bool	operator==( VectF3 const&, VectF3 const& );
bool	operator!=( VectF2 const&, VectF2 const& );
bool	operator!=( VectF3 const&, VectF3 const& );
//  2D operations
VectF2	operator+( VectF2 const&, VectF2 const& );
VectF2&	operator+=( VectF2&, VectF2 const& );
VectF2	operator-( VectF2 const&, VectF2 const& );
VectF2&	operator-=( VectF2&, VectF2 const& );
// 3D operations
VectF3	operator+( VectF3 const&, VectF3 const& );
VectF3&	operator+=( VectF3&, VectF3 const& );
VectF3	operator-( VectF3 const&, VectF3 const& );
VectF3&	operator-=( VectF3&, VectF3 const& );
// 2D vector - scalar operations
VectF2	operator+( VectF2 const&, float );
VectF2	operator+( float, VectF2 const& );
VectF2&	operator+=( VectF2&, float );
VectF2	operator-( VectF2 const&, float );
VectF2	operator-( float, VectF2 const& );
VectF2&	operator-=( VectF2&, float );
VectF2	operator*( VectF2 const&, float );
VectF2	operator*( float, VectF2 const& );
VectF2&	operator*=( VectF2&, float );
VectF2	operator/( VectF2 const&, float );
VectF2&	operator/=( VectF2&, float );
// 3D vector - scalar operations
VectF3	operator+( VectF3 const&, float );
VectF3	operator+( float, VectF3 const& );
VectF3&	operator+=( VectF3&, float );
VectF3	operator-( VectF3 const&, float );
VectF3	operator-( float, VectF3 const& );
VectF3&	operator-=( VectF3&, float );
VectF3	operator*( VectF3 const&, float );
VectF3	operator*( float, VectF3 const& );
VectF3&	operator*=( VectF3&, float );
VectF3	operator/( VectF3 const&, float );
VectF3&	operator/=( VectF3&, float );
// dot product
float	operator*( VectF2 const&, VectF2 const& );
float	operator*( VectF3 const&, VectF3 const& );
// cross product
float	operator^( VectF2 const&, VectF2 const& );
VectF3	operator^( VectF3 const&, VectF3 const& );
VectF3&	operator^=( VectF3&, VectF3 const& );

std::ostream& operator<<( std::ostream&, VectF2 const& );
std::ostream& operator<<( std::ostream&, VectF3 const& );
std::ostream& operator<<( std::ostream&, VectUI3 const& );

// lenght of a vector
float	getAbs( VectF2 const& );
float	getAbs( VectF3 const& );

VectF2	normalize( VectF2 const& );
VectF3	normalize( VectF3 const& );

VectF3	getNormal( VectF3 const&, VectF3 const&, VectF3 const&, bool = true );
VectF3	getNormal( std::vector<VectF3> const&, bool = true );
VectF3	getNormal( std::array<VectF3,3> const&, bool = true );
// checking CW or CCW orientation of 3 vertex coordinates
bool	isCCWorient( VectF3 const&, VectF3 const&, VectF3 const&, VectF3 const );
bool	isCCWorient( std::vector<VectF3> const&, VectF3 const );
bool	isCCWorient( std::array<VectF3,3> const&, VectF3 const );
bool	isCWorient( VectF3 const&, VectF3 const&, VectF3 const&, VectF3 const );
bool	isCWorient( std::vector<VectF3> const&, VectF3 const );
bool	isCWorient( std::array<VectF3,3> const&, VectF3 const );

// width in radiants of a vertex
float	width( VectF2 const&, VectF2 const&, VectF2 const& );
// check if a 2D vector is inside a triangle
bool	triangleContainmentTest( VectF2 const&, VectF2 const&, VectF2 const&, VectF2 const& );

template <uint32_t RANK>
class Matrix {
	public:
		Matrix( void ) noexcept = default;
		Matrix( float ) noexcept;
		explicit Matrix( std::array<std::array<float,RANK>,RANK> const&, bool = true ) noexcept;
		explicit Matrix( std::array<float,RANK * RANK> const& inputData ) noexcept : _data(inputData) {};
		Matrix( Matrix const& ) noexcept = default;
		Matrix( Matrix&& ) noexcept = default;
		Matrix& operator=( Matrix const& ) noexcept = default;
		Matrix& operator=( Matrix&& ) noexcept = default;
		~Matrix( void ) = default;

		float&			at( uint32_t, uint32_t );
		float const&	at( uint32_t, uint32_t ) const;
		float const*	data( void ) const noexcept;
		Matrix			transpose( void ) const noexcept;

		void	operator+=( Matrix const& ) noexcept;
		void	operator*=( Matrix const& ) noexcept;

	private:
		std::array<float,RANK * RANK> _data;
};

template <uint32_t RANK>
Matrix<RANK>::Matrix( float value ) noexcept {
	for(uint32_t i = 0; i < this->_data.size(); i++)
		this->_data[i] = value;
}

// column vector: N vertical vectors, row vector: N orizontal vectors
template <uint32_t RANK>
Matrix<RANK>::Matrix( std::array<std::array<float,RANK>,RANK> const& columnVectors, bool columnMajor ) noexcept {
	for (uint32_t row=0; row<RANK; row++) {
		for (uint32_t col=0; col<RANK; col++) {
			if (columnMajor)
				this->at(row, col) = columnVectors[col][row];
			else
				this->at(row, col) = columnVectors[row][col];
		}
	}
}

template <uint32_t RANK>
float& Matrix<RANK>::at( uint32_t row, uint32_t col ) {
	if ((row * RANK + col) >= this->_data.size())
		throw MathException("Index out of bounds");
	return this->_data[row * RANK + col];
}

template <uint32_t RANK>
float const& Matrix<RANK>::at( uint32_t row, uint32_t col ) const {
	if (row * RANK + col >= this->_data.size())
		throw MathException("Index out of bounds");
	return this->_data[row * RANK + col];
}

template <uint32_t RANK>
float const* Matrix<RANK>::data( void ) const noexcept {
	return this->_data.data();
}

template <uint32_t RANK>
Matrix<RANK> Matrix<RANK>::transpose( void) const noexcept {
	Matrix<RANK> trans = *this;
	for (uint32_t row=0; row<RANK; row++) {
		for (uint32_t col=row+1U; col<RANK; col++)
			std::swap(trans.at(row, col), trans.at(col, row));
	}
	return trans;
}

template <uint32_t RANK>
void Matrix<RANK>::operator+=( Matrix const& other ) noexcept {
	for (uint32_t i=0; i<this->_data.size(); i++)
		this->_data[i] += other._data[i];
}

template <uint32_t RANK>
void Matrix<RANK>::operator*=( Matrix const& other ) noexcept {
	for (uint32_t row=0; row<RANK; row++) {
		for (uint32_t col=0; col<RANK; col++) {
			float sumProd = 0;
			for (uint32_t k=0; k<RANK; k++)
				sumProd += this->at(row, k) * other.at(k, col);
			this->at(row, col) = sumProd;
		}
	}
}

template <uint32_t RANK>
Matrix<RANK> operator+( Matrix<RANK> const& m1, Matrix<RANK> const& m2 ) {
	Matrix<RANK> sum(0.0f);

	for (uint32_t row=0; row<RANK; row++) {
		for (uint32_t col=0; col<RANK; col++)
			sum.at(row, col) = m1.at(row, col) + m2.at(row, col);
	}
	return sum;
}

template <uint32_t RANK>
Matrix<RANK> operator*( Matrix<RANK> const& m1, Matrix<RANK> const& m2 ) {
	Matrix<RANK> product(0.0f);

	for (uint32_t row=0; row<RANK; row++) {
		for (uint32_t col=0; col<RANK; col++) {
			for (uint32_t k=0; k<RANK; k++)
				product.at(row, col) += m1.at(row, k) * m2.at(k, col);
		}
	}
	return product;
}

template <uint32_t RANK>
std::ostream& operator<<( std::ostream& os, Matrix<RANK> const& mat) {
	for (uint32_t row=0; row<RANK; row++) {
		for (uint32_t col=0; col<RANK; col++)
			os << mat.at(row, col) << " ";
		os << std::endl;
	}
	return os;
}

using Matrix4 = Matrix<4U>;

Matrix4 idMat( void );
Matrix4 transMat( VectF3 const& );
Matrix4 transMat( float );
Matrix4 scaleMat( VectF3 const& );
Matrix4 scaleMat( float );
Matrix4 rotationMat( float, VectF3 const& );

float	toRadiants( float );
float	toDegrees( float );
float	randomFloat( void );


struct Quatern {
	Quatern( void ) = default;
	Quatern( float _w, float _x, float _y, float _z ) :
		w(_w), x(_x), y(_y), z(_z) {};
	Quatern( float, VectF3 const& );
	~Quatern( void ) = default;

	Quatern		conjugate( void ) const noexcept;
	Quatern		inverse( void ) const;
	Matrix4		getMatrix( void ) const noexcept;
	VectF3		vector( void ) const noexcept;
	Quatern&	operator+=( Quatern const& ) noexcept;
	Quatern&	operator-=( Quatern const& ) noexcept;
	Quatern&	operator*=( Quatern const& ) noexcept;

	float w;
	float x;
	float y;
	float z;
};


bool	operator==( Quatern const&, Quatern const& );
bool	operator!=( Quatern const&, Quatern const& );
Quatern	operator+( Quatern const&, Quatern const& );
Quatern	operator-( Quatern const&, Quatern const& );
// Hamilton product
Quatern	operator*( Quatern const&, Quatern const& );
// real - quaternion operations
Quatern	operator+( Quatern const&, float );
Quatern	operator+( float, Quatern const& );
Quatern	operator-( Quatern const&, float );
Quatern	operator-( float, Quatern const& );
Quatern	operator*( Quatern const&, float );
Quatern	operator*( float, Quatern const& );
Quatern	operator/( Quatern const&, float );
std::ostream& operator<<( std::ostream&, Quatern const& );

float	getAbs( Quatern const& );
