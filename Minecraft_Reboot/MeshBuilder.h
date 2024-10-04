#pragma once

#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <memory>

#include "ShaderManager.h"
#include "TextureManager.h"

#include "Quad.h"
#include "VoxelUtil.h"

using namespace glm;

using namespace std;

class MeshBuilder
{
private:
	GLuint VAO;
	int faceCount = 0;

	bool vboGenerated = false;
	
	vector<float> vertices;
	vector<unsigned int> indices;

	shared_ptr<ShaderProgram> shaderProgram;

	void addPointV3(vec3);
	void addPointV2(vec2);
	void addPointV1(float);
	void addQuadIndices(bool, int);

	int getTextureIndex(int, int);
public:
	MeshBuilder();

	void addQuad(const Quad, int, int, bool);

	void generateVBO();

	void cleanUp();

	void render();
};

