#pragma once

#include <glad/glad.h>
#include <vector>

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

	void Generate(GLuint, GLuint, GLuint, GLuint, unsigned char*);

	void Use() const;
};

