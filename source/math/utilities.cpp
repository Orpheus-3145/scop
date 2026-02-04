#include <random>
#include <cmath>

#include "math/utilities.hpp"


float toRadiants( float degrees ) {
	return degrees * M_PI / 180.f;
}

float toDegrees( float radiants ) {
	return radiants * 180.f / M_PI;
}

float randomFloat( void ) {
	static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}
