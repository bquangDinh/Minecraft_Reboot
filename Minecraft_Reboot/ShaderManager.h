#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>

#include "Shader.h"
#include "ShaderProgram.h"

#define MAIN_SHADER_PROGRAM "main"

class Game;

using namespace std;

class ShaderManager
{
public:
	shared_ptr<ShaderProgram> getShaderProgram(string key);

	void cleanUp();

	static ShaderManager& getInstance();
private:
	// Prevent other classes from instantiating this class
	ShaderManager() {};

	static ShaderManager* _instance;

	unordered_map<string, shared_ptr<ShaderProgram>> shaderPrograms;

	shared_ptr<ShaderProgram> loadShaderProgram(string key, const char* vertexShaderFile, const char* fragmentShaderFile);
	shared_ptr<ShaderProgram> loadShaderProgram(string key, Shader vertexShader, Shader fragmentShader);
	shared_ptr<ShaderProgram> loadShaderProgram(string key, GLuint vertexShader, GLuint fragmentShader);

	friend Game;
};

