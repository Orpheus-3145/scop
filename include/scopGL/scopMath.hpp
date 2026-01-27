#pragma once
#include <array>
#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include <cstdint>

#include "exceptions.hpp"

static const float F_ZERO = 1e-6f;

struct VectF2D {
	float x;
	float y;

	static VectF2D				from_array( std::array<float,2> const& ) noexcept;
	static std::array<float,2>	to_array( VectF2D const& ) noexcept;
};

struct VectF3D {
	float x;
	float y;
	float z;

	static VectF3D				from_array( std::array<float,3> const& ) noexcept;
	static VectF3D				from_vector( std::vector<float> const& );
	static std::array<float,3>	to_array( VectF3D const& ) noexcept;
};

struct VectUI3D {
	uint32_t i1;
	uint32_t i2;
	uint32_t i3;

	static VectUI3D 				from_array( std::array<uint32_t,3> const& ) noexcept;
	static VectUI3D					from_vector( std::vector<uint32_t> const& );
	static std::array<uint32_t,3>	to_array( VectUI3D const& ) noexcept;
};
// NB add math exceptions
bool	operator==( VectF2D const&, VectF2D const& );
bool	operator==( VectF3D const&, VectF3D const& );
bool	operator!=( VectF2D const&, VectF2D const& );
bool	operator!=( VectF3D const&, VectF3D const& );
// sum vectors
VectF2D	operator+( VectF2D const&, VectF2D const& );
VectF3D	operator+( VectF3D const&, VectF3D const& );
// diff vectors
VectF2D	operator-( VectF2D const&, VectF2D const& );
VectF3D	operator-( VectF3D const&, VectF3D const& );
// vector - scalar operations
VectF2D	operator+( VectF2D const&, float );
VectF2D	operator+( float, VectF2D const& );
VectF2D	operator-( VectF2D const&, float );
VectF2D	operator-( float, VectF2D const& );
VectF2D	operator*( VectF2D const&, float );
VectF2D	operator*( float, VectF2D const& );
VectF2D	operator/( VectF2D const&, float );
VectF2D	operator/( float, VectF2D const& );
VectF3D	operator+( VectF3D const&, float );
VectF3D	operator+( float, VectF3D const& );
VectF3D	operator-( VectF3D const&, float );
VectF3D	operator-( float, VectF3D const& );
VectF3D	operator*( VectF3D const&, float );
VectF3D	operator*( float, VectF3D const& );
VectF3D	operator/( VectF3D const&, float );
VectF3D	operator/( float, VectF3D const& );
// dot product
float	operator*( VectF2D const&, VectF2D const& );
float	operator*( VectF3D const&, VectF3D const& );
// cross product
float	operator^( VectF2D const&, VectF2D const& );
VectF3D	operator^( VectF3D const&, VectF3D const& );

float	getAbs( VectF2D const& );
float	getAbs( VectF3D const& );

VectF2D	normalize( VectF2D const& );
VectF3D	normalize( VectF3D const& );

VectF3D	getNormal( VectF3D const&, VectF3D const&, VectF3D const&, bool = true );
VectF3D	getNormal( std::vector<VectF3D> const&, bool = true );
VectF3D	getNormal( std::array<VectF3D,3> const&, bool = true );
// checking CW or CCW orientation of 3 vertex coordinates
bool	isCCWorient( VectF3D const&, VectF3D const&, VectF3D const& );
bool	isCCWorient( std::vector<VectF3D> const& );
bool	isCCWorient( std::array<VectF3D,3> const& );
bool	isCWorient( VectF3D const&, VectF3D const&, VectF3D const& );
bool	isCWorient( std::vector<VectF3D> const& );
bool	isCWorient( std::array<VectF3D,3> const& );

// width in radiants of a vertex
float	width( VectF2D const&, VectF2D const&, VectF2D const& );
// check if a 2D vector is inside a triangle
bool	triangleContainmentTest( VectF2D const&, VectF2D const&, VectF2D const&, VectF2D const& );

std::ostream& operator<<( std::ostream&, VectF2D const& );
std::ostream& operator<<( std::ostream&, VectF3D const& );
std::ostream& operator<<( std::ostream&, VectUI3D const& );


class Matrix4 {
	public:
		Matrix4( void ) noexcept = default;
		Matrix4( float ) noexcept;
		explicit Matrix4( std::array<float,4> const&, std::array<float,4> const&, std::array<float,4> const&, std::array<float,4> const& ) noexcept;
		explicit Matrix4( std::array<float,16> const& inputData ) noexcept : _data(inputData) {};
		Matrix4( Matrix4 const& ) noexcept = default;
		Matrix4( Matrix4&& ) noexcept = default;
		Matrix4& operator=( Matrix4 const& ) noexcept = default;
		Matrix4& operator=( Matrix4&& ) noexcept = default;
		~Matrix4( void ) = default;

		float&			at( uint32_t, uint32_t );
		float const&	at( uint32_t, uint32_t ) const;
		float const*	data( void ) const noexcept;

		friend Matrix4	operator+( Matrix4 const&, Matrix4 const& );
		void			operator+=( Matrix4 const& ) noexcept;
		friend Matrix4	operator*( Matrix4 const&, Matrix4 const& );
		void			operator*=( Matrix4 const& ) noexcept;

	private:
		std::array<float,16> _data;
};

Matrix4 createIdMat( void );
Matrix4 transMat( std::array<float,3> );
Matrix4 transMat( float );
Matrix4 scaleMat( std::array<float,3> );
Matrix4 scaleMat( float );
Matrix4 rotationMat( float, std::array<float,3> );
Matrix4	projectionMatFinite( float , float , float , float );
Matrix4	projectionMatInfinite( float , float , float );
Matrix4 operator+( Matrix4 const&, Matrix4 const& );
Matrix4 operator*( Matrix4 const&, Matrix4 const& );

float	toRadiants( float );
float	toDegrees( float );
float	randomFloat( void );
