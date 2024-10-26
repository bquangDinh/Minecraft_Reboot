#include "MathUtil.h"

int MathUtil::flattenIndex(const vec2 pos, const vec2 dimensions) {
	return pos.x + pos.y * dimensions.x;
}

int MathUtil::flattenIndex(const vec3 pos, const vec3 dimensions) {
	return pos.x + pos.y * dimensions.x + pos.z * dimensions.x * dimensions.y;
}

bool MathUtil::isPowerOf2(const int n) {
	return (n & (n - 1)) == 0;
}

float MathUtil::getRandomFloat(const float min, const float max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);

	return dis(gen);
}