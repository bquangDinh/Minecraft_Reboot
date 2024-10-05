#pragma once

#include <glm/glm.hpp>

using namespace glm;

class MathUtil
{
public:
	static int flattenIndex(const vec2 pos, const vec2 dimensions);
	static int flattenIndex(const vec3 pos, const vec3 dimensions);
};

