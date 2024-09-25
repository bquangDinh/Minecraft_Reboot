#pragma once

#include <glad/glad.h>
#include "FileUtils.h"

class Shader
{
public:
	// Shader ID
	GLuint ID;

	Shader(const char*, GLenum);

	Shader();

	~Shader();
};

