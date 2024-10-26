#pragma once

#include <glm/glm.hpp>
#include "VoxelUtil.h"

using namespace glm;

class Voxel
{
public:
	bool transparent;
	int type;
	vec3 pos;

	Voxel();
	//Voxel(int type, bool transparent);
	Voxel(int type, bool transparent, vec3 pos);
	Voxel(const Voxel& other);

	Voxel& operator=(const Voxel& other);
	bool operator==(const Voxel& other);

	bool equals(const Voxel& other);
	bool isSolid();
};

