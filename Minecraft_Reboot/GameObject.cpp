#include "GameObject.h"

GameObject::GameObject(): 
	gameStates(&GameStates::getInstance()),
	shaderManager(&ShaderManager::getInstance())
{}

GameObject::~GameObject()
{
	gameStates = nullptr;

	shaderManager = nullptr;
}

void GameObject::setPosition(glm::vec3 position)
{
	this->position = position;
}