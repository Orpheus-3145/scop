#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <iostream>


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
