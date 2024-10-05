#pragma once

#include "VoxelUtil.h"

class Voxel
{
public:
	bool transparent;
	int type;

	Voxel();
	Voxel(int type, bool transparent);

	Voxel& operator=(const Voxel& other);
	bool operator==(const Voxel& other);

	bool equals(const Voxel& other);
	bool isSolid();
};

