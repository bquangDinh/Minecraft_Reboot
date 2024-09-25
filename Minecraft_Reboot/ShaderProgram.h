#pragma once

#include <glad/glad.h>

// Mostly stuffs to calculate matrices
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"

using namespace std;

class ShaderProgram
{
public:
	// Id of the shader program
	GLuint ID;

	// Construct a shader program with two vextex and fragment shader GLuint given
	ShaderProgram(const GLuint, const GLuint);

	// Construct a shader program given two vextex and fragment Shader objects
	ShaderProgram(const Shader, const Shader);

	// Construct a shader program from another shader program by cloning
	ShaderProgram(const GLuint);

	// Construct a shader program provided with two source vextex and fragment shaders
	ShaderProgram(const char*, const char*);

	// Default
	ShaderProgram();

	~ShaderProgram();
		
	// Tell OpenGL to use this shader program
	void Use();

	// Set Values
	void SetMatrix4(const char*, const glm::mat4, const bool transpose = false);
	void SetVector2(const char*, const glm::vec2);
	void SetVector3(const char*, const glm::vec3);
	void SetFloat(const char*, const float);

private:
	void checkProgramLinkSuccess(const GLuint, const GLuint);
};

