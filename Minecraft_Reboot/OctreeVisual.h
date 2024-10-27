#pragma once

#include "GameObject.h"
#include "Octree.h"

class OctreeVisual : public GameObject
{
private:
	Octree* octree;

	mat4 model;

	GLuint VAO;

	int faceCount, verticesCount, indicesCount;

	std::vector<float> vertices;

	std::vector<unsigned int> indices;

	bool vboGenerated, isOctreeSet;

	void addQuad(const vec3& a, const vec3& b, const vec3& c, const vec3& d);

	void onOctreeTraverse(Node* node);

	void generateVBO();
public:
	OctreeVisual(Octree** octree, const vec3 position);
	~OctreeVisual();

	void setOctree(Octree** octree);

	void init() override;
	void update(float deltaTime) override;
	void render(float deltaTime) override;
	void destroy() override;
};

