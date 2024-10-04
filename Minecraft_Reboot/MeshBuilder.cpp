#include "MeshBuilder.h"

MeshBuilder::MeshBuilder(): VAO(0) {
	// Load shader program
	shaderProgram = ShaderManager::getInstance().getShaderProgram(MAIN_SHADER_PROGRAM);
}

void MeshBuilder::addQuad(const Quad q, int width, int height, bool backface) {
	//prevent stupid and culling method duplicate more than one texture !
	if (width <= 1.0f) width = 0;
	if (height <= 1.0f) height = 0;

	// Obtain texture index for this quad
	int textureIndex = getTextureIndex(q.getType(), q.getFace());

	/* Add point 1 */
	// Add quad vertices
	addPointV3(q.p1);

	// Add texture coordinates
	addPointV2(vec2(0.0f + width, 1.0f + height));

	// Add texture index of this quad
	addPointV1(textureIndex);

	/* Add point 2 */
	addPointV3(q.p2);

	addPointV2(vec2(1.0f + width, 1.0f));

	addPointV1(textureIndex);

	/* Add point 3 */
	addPointV3(q.p3);

	addPointV2(vec2(1.0f, 0.0f));

	addPointV1(textureIndex);

	/* Add point 4 */
	addPointV3(q.p4);

	addPointV2(vec2(0.0f, 0.0f + height));

	addPointV1(textureIndex);

	// Add quad indices
	addQuadIndices(backface, faceCount);

	// Update face count
	faceCount++;
}

void MeshBuilder::addPointV3(const vec3 point) {
	vertices.push_back(point.x);
	vertices.push_back(point.y);
	vertices.push_back(point.z);
}

void MeshBuilder::addPointV2(const vec2 point) {
	vertices.push_back(point.x);
	vertices.push_back(point.y);
}

void MeshBuilder::addPointV1(const float point) {
	vertices.push_back(point);
}

void MeshBuilder::addQuadIndices(const bool backface, const int offset) {
	if (backface) {
		indices.push_back(0 + 4 * offset);
		indices.push_back(1 + 4 * offset);
		indices.push_back(2 + 4 * offset);

		indices.push_back(2 + 4 * offset);
		indices.push_back(3 + 4 * offset);
		indices.push_back(0 + 4 * offset);
	}
	else {
		indices.push_back(0 + 4 * offset);
		indices.push_back(2 + 4 * offset);
		indices.push_back(1 + 4 * offset);

		indices.push_back(2 + 4 * offset);
		indices.push_back(0 + 4 * offset);
		indices.push_back(3 + 4 * offset);
	}
}

int MeshBuilder::getTextureIndex(int type, int face) {
	if (type == VOXEL_TYPE::GRASS) {
		if (face == SIDES::TOP) {
			return 0;
		}
		else if (face == SIDES::BOTTOM) {
			return 2;
		}
		else {
			return 3;
		}
	}

	if (type == VOXEL_TYPE::DIRT) {
		return 2;
	}

	if (type == VOXEL_TYPE::ROCK) {
		return 1;
	}

	return 2;
}

void MeshBuilder::generateVBO() {
	if (vboGenerated) {
		cout << "VBO already generated!" << endl;

		return;
	}

	// Generate VAO
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);

	// Generate VBO
	GLuint VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	// Generate EBO
	GLuint EBO;

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Set vertex attribute pointers
	// For position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	// For texture coordinates attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// For texture index attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

	//// Unbind VAO
	glBindVertexArray(0);

	//// Unbind VBO
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// Unbind EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vboGenerated = true;
}

void MeshBuilder::cleanUp() {
	if (!vboGenerated) {
		cout << "VBO not generated!" << endl;

		return;
	}

	glDeleteVertexArrays(1, &VAO);

	vboGenerated = false;

	faceCount = 0;

	vertices.clear();

	indices.clear();
}

void MeshBuilder::render() {
	if (!vboGenerated) {
		cout << "VBO not generated!" << endl;

		return;
	}

	TextureManager::getInstance().getTextureArray(MAIN_TEXTURE_ARRAY)->Use();

	shaderProgram->Use();

	mat4 model = mat4(1.0f);

	shaderProgram->SetMatrix4("model", model);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glBindVertexArray(0);
}