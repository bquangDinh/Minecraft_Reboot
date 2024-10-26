#pragma once

#include "glm/glm.hpp"
#include "memory"

#include "Voxel.h"

using namespace glm;

struct BoundingBox {
	vec3 center; // center point of the bounding box

	float size; // size of the bounding box all in 3 dimensions

	BoundingBox(const vec3& center, float size);

	// Check if the point is inside the bounding box
	bool contains(const vec3& point) const;

	// Get the smaller bounding box of the 8 children bounding boxes inside this bounding box
	// index is a 3-bit number where each bit represents the direction of the child bounding box
	// 0 is negative direction, 1 is positive direction
	// 0b000 is the child bounding box at the negative direction of all 3 axes--
	// 0b111 is the child bounding box at the positive direction of all 3 axes
	BoundingBox childBox(int index);
};

struct Node {
	BoundingBox box;

	Node* children[8];

	std::unique_ptr<Voxel> voxel;

	Node(const BoundingBox& box);

	// Check if the node is a leaf node
	bool isLeaf() const;
};

class Octree
{
private:
	Node* root;

	void insert(Node* node, const vec3& position, std::unique_ptr<Voxel> voxel);

	Voxel* find(Node* node, const vec3& position);

	void clear(Node** node);

	void subdivide(Node* node);

	size_t calculateMemoryUsage(Node* node);
public:
	Octree(const BoundingBox& box);

	~Octree();

	void insert(const vec3& position, std::unique_ptr<Voxel> voxel);

	Voxel* find(const vec3& position);

	void clear();

	void printMemoryUsage();
};

