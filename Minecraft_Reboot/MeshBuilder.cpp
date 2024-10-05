#include "MeshBuilder.h"

MeshBuilder::MeshBuilder(): VAO(0) {
	// Load shader program
	shaderProgram = ShaderManager::getInstance().getShaderProgram(MAIN_SHADER_PROGRAM);
}

void MeshBuilder::addQuad(const Quad q, int width, int height, bool backface) {
	// Obtain texture index for this quad
	int textureIndex = getTextureIndex(q.getType(), q.getFace());

	/* Add point 1 */
	// Add quad vertices
	addPointV3(q.p1);

	// Add texture coordinates
	addPointV2(vec2(0.0f, height));

	// Add texture index of this quad
	addPointV1(textureIndex);

	/* Add point 2 */
	addPointV3(q.p2);

	addPointV2(vec2(width, height));

	addPointV1(textureIndex);

	/* Add point 3 */
	addPointV3(q.p3);

	addPointV2(vec2(width, 0.0f));

	addPointV1(textureIndex);

	/* Add point 4 */
	addPointV3(q.p4);

	addPointV2(vec2(0.0f, 0.0f));

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
		indices.push_back(2 + 4 * offset);
		indices.push_back(1 + 4 * offset);
		indices.push_back(0 + 4 * offset);

		indices.push_back(0 + 4 * offset);
		indices.push_back(3 + 4 * offset);
		indices.push_back(2 + 4 * offset);
	}
	else {
		indices.push_back(0 + 4 * offset);
		indices.push_back(1 + 4 * offset);
		indices.push_back(2 + 4 * offset);

		indices.push_back(2 + 4 * offset);
		indices.push_back(3 + 4 * offset);
		indices.push_back(0 + 4 * offset);
	}
}

int MeshBuilder::getTextureIndex(int type, int face) {
	if (type == VOXEL_TYPE::GRASS) {
		if (face == SIDES::TOP) {
			return 39;
		}
		else if (face == SIDES::BOTTOM) {
			return 13;
		}
		else {
			return 12;
		}
	}

	if (type == VOXEL_TYPE::DIRT) {
		return 13;
	}

	if (type == VOXEL_TYPE::ROCK) {
		return 14;
	}

	if (type == VOXEL_TYPE::WATER) {
		return 192;
	}

	return 12;
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

	vboGenerated = true;

#ifdef ENABLE_LOGGING
	printReports();
#endif // ENABLE_LOGGING
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

	glBindVertexArray(0);
}

void MeshBuilder::printReports() {
	cout << "Vertices Element Count: " << vertices.size() << "(" << sizeof(float) * vertices.size() << " bytes)" << endl;

	cout << "Indices Element Count: " << indices.size() << "(" << sizeof(unsigned int) * indices.size() << " bytes)" << endl;

	cout << "Face Count: " << faceCount << endl;

	cout << "Vertices Count: " << 4 * faceCount << endl;
}

//void Texture2DArray::Generate(GLuint width, GLuint height, GLuint texelsX, GLuint texelsY, const int nrChannels, unsigned char* data) {
//	this->width = width;
//
//	this->height = height;
//
//	unsigned int tileW = width / texelsX;
//
//	unsigned int tileH = height / texelsY;
//
//	const int tilesCount = texelsX * texelsY;
//
//	if (nrChannels == 4) {
//		internalFormat = GL_RGBA8;
//		imageFormat = GL_RGBA;
//	}
//	else {
//		internalFormat = GL_RGB8;
//		imageFormat = GL_RGB;
//	}
//
//	// Create the texture
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);
//
//	// Width and Height is texels, not pixels
//	// Allocate storage for the texture
//	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, texelsX, texelsY, tilesCount, 0, imageFormat, GL_UNSIGNED_BYTE, nullptr);
//
//	/* Load each tile data to OpenGL */
//	std::vector<unsigned char> tiles(tileW * tileH * nrChannels);
//
//	// Each tile has 4 channels, thus tileW * nrChannels
//	const int tileSizeX = tileW * nrChannels;
//
//	const int rows = tileH;
//
//	printf("tileSizeX: %d, rows: %d\n", tileSizeX, rows);
//
//	// Load data for each tile
//	for (int iy = 0; iy < texelsY; iy++) {
//		for (int ix = 0; ix < texelsX; ix++) {
//			// At the tile[X][Y] position
//			unsigned char* ptr = data + ix * tileSizeX + iy * tileSizeX * texelsX * tileH;
//
//			// Copy the tile data to the vector
//			for (int i = 0; i < tileH; i++) {
//				std::copy(ptr + i * tileSizeX * texelsX, ptr + i * tileSizeX * texelsX + tileSizeX, tiles.begin() + i * tileSizeX);
//			}
//
//			// Flat out the index x,y to a single index
//			int index = ix + iy * texelsX;
//
//			std::cout << "ix: " << ix << ", iy: " << iy << ", index: " << index << "\n" << std::endl;
//
//			// Load the tile data to OpenGL
//			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, tileW, tileH, 1, imageFormat, GL_UNSIGNED_BYTE, tiles.data());
//		}
//	}
//
//	// Set the texture wrapping/filtering options
//	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapS);
//	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapT);
//	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, wrapR);
//
//	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, filterMin);
//	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, filterMag);
//
//	// Generate Mipmaps
//	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
//
//	// Unbind the texture
//	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
//}