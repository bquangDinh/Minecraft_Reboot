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

void Texture2DArray::Generate(GLuint width, GLuint height, int texelsX, int texelsY, const int nrChannels, unsigned char* data) {
	this->width = width;
	
	this->height = height;

	// tile is one block in the texture atlas
	// texel is the unit for texture
	// Just think it's like how many tiles are in the grid per dimension
	int tileW = width / texelsX; // width in pixels of a tile

	int tileH = height / texelsY; // height in pixels of a tile

	const int tilesCount = texelsX * texelsY; // the number of tiles totally

	// Check if the data stores the alpha channel
	// Note that the data I read is interleaved-structured
	// Mean the color data for each pixel is stored next to each other
	// Ex: [R1 G1 B1 A1] [R2 G2 B2 A2] ...
	if (nrChannels == 4) {
		internalFormat = GL_RGBA; // Internal compressed format
		imageFormat = GL_RGBA; // Bit Depth of the image
	}
	else {
		internalFormat = GL_RGB;
		imageFormat = GL_RGB;
	}

	// Create the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);

	// Width and Height for glTexImage3D() are texels, not pixels
	// Allocate storage for the texture
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, texelsX, texelsY, tilesCount, 0, imageFormat, GL_UNSIGNED_BYTE, nullptr);

	/* Load each tile data to OpenGL */
	// this vectore stores a tile data
	std::vector<unsigned char> tile(tileW * tileH * nrChannels);

	// tileSizeX represents how many bytes of each tile horizontally
	// Each tile has nrChannel channels, thus tileW * nrChannels
	const int tileSizeX = tileW * nrChannels;

	// tileSizeY represents the number of rows of the bytes structure of the tile
	const int totalBytesEachRow = tileSizeX * texelsX;

	// Load data for each tile
	for (int iy = 0; iy < texelsY; iy++) {
		for (int ix = 0; ix < texelsX; ix++) {
			// At the tile[X][Y] position
			unsigned char* ptr = data + ix * tileSizeX + iy * totalBytesEachRow * tileH;

			// Copy the tile data to the vector
			for (int i = 0; i < tileH; i++) {
				std::copy(ptr + i * totalBytesEachRow, ptr + i * totalBytesEachRow + tileSizeX, tile.begin() + i * tileSizeX);
			}

			// Flat out the index x,y to a single index
			int index = ix + iy * texelsX;

			// Load the tile data to OpenGL
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, tileW, tileH, 1, imageFormat, GL_UNSIGNED_BYTE, tile.data());
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

	// Clean up the data
	tile.clear();
}

void Texture2DArray::Use() const {
	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);
}