#pragma once

#include <glm/glm.hpp>
#include "VoxelUtil.h"

using namespace glm;

class Quad
{
private:
	int type;
	bool transparent;
	int face;
public:
	vec3 p1, p2, p3, p4;

	Quad(vec3, vec3, vec3, vec3, int, int, bool);
	Quad();

	bool equals(const Quad) const;
	bool isTransparent() const;
	int getFace() const;
	int getType() const;

	void rotateQuadOrder90();
	void rotateQuadOrder90Counter();
};

