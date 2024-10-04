#include "Texture2DArray.h"

Texture2DArray::Texture2DArray():
	internalFormat(GL_RGBA8),
	imageFormat(GL_RGBA),
	wrapS(GL_REPEAT),
	wrapT(GL_REPEAT),
	wrapR(GL_REPEAT),
	filterMin(GL_NEAREST),
	filterMag(GL_NEAREST),
	width(0),
	height(0)
{
	glGenTextures(1, &ID);
}

void Texture2DArray::Generate(GLuint width, GLuint height, GLuint texelsX, GLuint texelsY, unsigned char* data) {
	this->width = width;
	
	this->height = height;

	unsigned int tileW = width / texelsX;

	unsigned int tileH = height / texelsY;

	const int nrChannels = 4; // R G B A

	const int tilesCount = texelsX * texelsY;

	// Create the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);

	// Width and Height is texels, not pixels
	// Allocate storage for the texture
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, texelsX, texelsY, tilesCount, 0, imageFormat, GL_UNSIGNED_BYTE, nullptr);

	/* Load each tile data to OpenGL */
	std::vector<unsigned char> tiles(tileW * tileH * nrChannels);

	// Each tile has 4 channels, thus tileW * nrChannels
	const int tileSizeX = tileW * nrChannels;

	const int rows = texelsX * tileSizeX;

	// Load data for each tile
	for (int iy = 0; iy < texelsY; iy++) {
		for (int ix = 0; ix < texelsX; ix++) {
			// At the tile[X][Y] position
			unsigned char* ptr = data + iy * rows + ix * tileSizeX;

			// Copy the tile data to the vector
			for (int i = 0; i < tileH; i++) {
				std::copy(ptr + i * rows, ptr + i * rows + tileSizeX, tiles.begin() + i * tileSizeX);
			}

			// Flat out the index x,y to a single index
			int index = ix + iy * texelsX;	

			// Load the tile data to OpenGL
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, tileW, tileH, 1, imageFormat, GL_UNSIGNED_BYTE, tiles.data());
		}
	}

	// Set the texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, wrapR);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, filterMin);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, filterMag);

	// Generate Mipmaps
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void Texture2DArray::Use() const {
	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);
}