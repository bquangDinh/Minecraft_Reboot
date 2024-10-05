#include "MathUtil.h"

int MathUtil::flattenIndex(const vec2 pos, const vec2 dimensions) {
	return pos.x + pos.y * dimensions.x;
}

int MathUtil::flattenIndex(const vec3 pos, const vec3 dimensions) {
	return pos.x + pos.y * dimensions.x + pos.z * dimensions.x * dimensions.y;
}