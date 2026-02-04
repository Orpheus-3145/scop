#include "math/matrix.hpp"
#include "exception.hpp"


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
