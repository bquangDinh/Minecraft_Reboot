#pragma once

#include <glm/glm.hpp>

#include "GameObject.h"
#include "Voxel.h"
#include "Quad.h"
#include "MeshGenerator.h"
#include "MeshBuilder.h"
#include "Octree.h"
#include "OctreeVisual.h"

using namespace glm;

enum MESHING_METHOD {
	STUPID,
	CULLING,
	GREEDY
};

class Chunk: public GameObject
{
private:
	static const float VOXEL_UNIT;

	Octree* voxelsData;

	vec3 dimensions;

	vec3 position;

	mat4 model;
		
	MeshBuilder* meshBuilder;

	OctreeVisual* octreeVisual;

	bool initialized;

	// Meshing algorithm
	void stupidMeshing();

	void cullingMeshing();

	void greedyMeshing();

	int getActualFaceIndex(int direction, bool backface);

	// Check if there is a block in that direction that blocks this face
	bool isFaceBlocking(vec3 pos, int direction, bool backface);

	// Check if the two voxels can be merged at the current position and the comparing position
	bool shouldMergeTwoVoxel(vec3 currentPos, vec3 comparingPos, int direction, bool backface);
public:
	tuple<int, int>* chunkCoord;

	bool pendingDeletion; // Flag to mark if the chunk should be deleted

	Chunk(const vec3, const vec3);

	~Chunk();

	void init() override;

	void update(float) override;

	void render(float) override;

	void destroy() override;
};

