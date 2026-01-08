#pragma once
#include <array>
#include <cmath>

#include "exceptions.hpp"

class Matrix4 {
	public:
		Matrix4( void ) noexcept {};
		Matrix4( float ) noexcept;
		explicit Matrix4( std::array<float,4> const&, std::array<float,4> const&, std::array<float,4> const&, std::array<float,4> const& ) noexcept;
		explicit Matrix4( std::array<float,16> const& inputData ) noexcept : _data(inputData) {};
		Matrix4( Matrix4 const& ) noexcept;
		Matrix4( Matrix4&& ) noexcept;
		Matrix4& operator=( Matrix4 const& ) noexcept;
		Matrix4& operator=( Matrix4&& ) noexcept;

		float&			at( unsigned int, unsigned int );
		float const&	at( unsigned int, unsigned int ) const;
		float const*	data( void ) const noexcept;

		Matrix4 operator+( Matrix4 const& ) const noexcept;
		void	operator+=( Matrix4 const& ) noexcept;
		Matrix4 operator*( Matrix4 const& ) const noexcept;
		void	operator*=( Matrix4 const& ) noexcept;
 
	private:
		std::array<float,16> _data;
};

Matrix4 createIdMat( void );
Matrix4 createTransMat( std::array<float,3> );
Matrix4 createTransMat( float );
Matrix4 createScaleMat( std::array<float,3> );
Matrix4 createScaleMat( float );
Matrix4 createRotationMat( float, std::array<float,3> );
