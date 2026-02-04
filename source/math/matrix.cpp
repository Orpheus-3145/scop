#include <cmath>

#include "math/matrix.hpp"


Matrix4 idMat( void ) {
	return Matrix4(std::array<float,16>{
		1.0f,  .0f,  .0f,  .0f,
		 .0f, 1.0f,  .0f,  .0f,
		 .0f,  .0f, 1.0f,  .0f,
		 .0f,  .0f,  .0f, 1.0f
	});
}

Matrix4 transMat( std::array<float,3> const& transArray ) {
	Matrix4 translation(std::array<float,16>{
		1.0f,  .0f,  .0f, transArray[0],
		 .0f, 1.0f,  .0f, transArray[1],
		 .0f,  .0f, 1.0f, transArray[2],
		 .0f,  .0f, 0.0f, 1.0f
	});
	return translation;
}

Matrix4 transMat( VectF3 const& transVect ) {
	Matrix4 translation(std::array<float,16>{
		1.0f,  .0f,  .0f, transVect.x,
		 .0f, 1.0f,  .0f, transVect.y,
		 .0f,  .0f, 1.0f, transVect.z,
		 .0f,  .0f, 0.0f, 1.0f
	});
	return translation;
}

Matrix4 transMat( float uniTranslation ) {
	Matrix4 translation(std::array<float,16>{
		1.0f,  .0f,  .0f, uniTranslation,
		 .0f, 1.0f,  .0f, uniTranslation,
		 .0f,  .0f, 1.0f, uniTranslation,
		 .0f,  .0f, 0.0f, 1.0f
	});
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

Matrix4 rotationMat( float tetha, VectF3 const& rotAxis ) {
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
	return rotation;
}
