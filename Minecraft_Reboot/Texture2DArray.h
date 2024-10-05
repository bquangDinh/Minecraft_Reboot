#pragma once

#include <glad/glad.h>
#include <vector>
#include <iostream>

class Texture2DArray
{
public:
	GLuint ID;
	GLuint internalFormat;
	GLuint imageFormat;
	GLuint wrapS, wrapT, wrapR;
	GLuint filterMin, filterMag;
	GLuint width, height;

	Texture2DArray();

	void Generate(GLuint width, GLuint height, int texelsX, int texelsY, const int nrChannels, unsigned char*);

	void Use() const;
};

