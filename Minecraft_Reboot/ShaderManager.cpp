#include "ShaderManager.h"
#include "Game.h"

shared_ptr<ShaderManager> ShaderManager::getInstance()
{
	static shared_ptr<ShaderManager> instance(new ShaderManager());	// Guaranteed to be destroyed.
																	// Instantiated on first use.

	return instance;
}

shared_ptr<ShaderProgram> ShaderManager::getShaderProgram(string key)
{
	if (shaderPrograms.find(key) == shaderPrograms.end())
	{
		return nullptr;
	}

	return shaderPrograms[key];
}

void ShaderManager::cleanUp()
{
	for (auto it = shaderPrograms.begin(); it != shaderPrograms.end(); it++)
	{
		it->second->Destroy();
	}

	shaderPrograms.clear();
}

shared_ptr<ShaderProgram> ShaderManager::loadShaderProgram(string key, const char* vertexShaderFile, const char* fragmentShaderFile)
{
	Shader vertexShader(vertexShaderFile, GL_VERTEX_SHADER);
	Shader fragmentShader(fragmentShaderFile, GL_FRAGMENT_SHADER);

	return loadShaderProgram(key, vertexShader, fragmentShader);
}

shared_ptr<ShaderProgram> ShaderManager::loadShaderProgram(string key, Shader vertexShader, Shader fragmentShader)
{
	shaderPrograms[key] = make_shared<ShaderProgram>(vertexShader, fragmentShader);

	return shaderPrograms[key];
}

shared_ptr<ShaderProgram> ShaderManager::loadShaderProgram(string key, GLuint vertexShader, GLuint fragmentShader)
{
	shaderPrograms[key] = make_shared<ShaderProgram>(vertexShader, fragmentShader);

	return shaderPrograms[key];
}