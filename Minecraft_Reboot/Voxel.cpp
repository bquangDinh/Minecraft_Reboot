#include "Voxel.h"

Voxel::Voxel(): transparent(true), type(VOXEL_TYPE::AIR) {}

Voxel::Voxel(int type, bool transparent) : type(type), transparent(transparent) {}

Voxel& Voxel::operator=(const Voxel& other) {
	type = other.type;
	transparent = other.transparent;
	return *this;
}

bool Voxel::operator==(const Voxel& other) {
	return type == other.type && transparent == other.transparent;
}

bool Voxel::equals(const Voxel& other) {
	return type == other.type && transparent == other.transparent;
}

bool Voxel::isSolid() {
	return type != VOXEL_TYPE::AIR;
}