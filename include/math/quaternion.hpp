#pragma once

#include "math/vector.hpp"
#include "math/matrix.hpp"


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
