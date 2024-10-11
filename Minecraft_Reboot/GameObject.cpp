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