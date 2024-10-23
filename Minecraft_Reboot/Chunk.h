#pragma once

#include <glm/glm.hpp>

#include "GameObject.h"
#include "Voxel.h"
#include "Quad.h"
#include "MeshGenerator.h"
#include "MeshBuilder.h"

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

	const Voxel* voxelsData;

	vec3 dimensions;

	vec3 position;

	mat4 model;
		
	MeshBuilder* meshBuilder;

	float** sharedVerticesBuffer;

	unsigned int** sharedIndicesBuffer;

	int startVerticesIndex, startIndicesIndex;

	bool initialized;

	bool isSharedBuffersUsed;

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
	Chunk(const vec3, const vec3);

	~Chunk();

	void useSharedBuffers(float** sharedBuffer, const int startVerticesIndex, unsigned int** sharedIndicesBuffer, const int startIndicesIndex);

	void init() override;

	void update(float) override;

	void render(float) override;

	void destroy() override;
};

