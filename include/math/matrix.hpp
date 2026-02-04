#pragma once
#include <array>
#include <iostream>
#include <cstdint>

#include "math/vector.hpp"


template <uint32_t RANK>
class Matrix {
	public:
		Matrix( float ) noexcept;
		explicit Matrix( std::array<std::array<float,RANK>,RANK> const&, bool = true ) noexcept;
		explicit Matrix( std::array<float,RANK * RANK> const& inputData ) noexcept : _data(inputData) {};
		Matrix( void ) noexcept = default;
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

#include "math/matrix.tpp"

using Matrix4 = Matrix<4U>;

Matrix4 idMat( void );
Matrix4 transMat( VectF3 const& );
Matrix4 transMat( float );
Matrix4 scaleMat( VectF3 const& );
Matrix4 scaleMat( float );
Matrix4 rotationMat( float, VectF3 const& );
