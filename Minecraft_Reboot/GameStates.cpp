#include "GameStates.h"
#include "Game.h"

GameStates& GameStates::getInstance()
{
	static GameStates instance; // Guaranteed to be destroyed.
								// Instantiated on first use.

	return instance;
}

int GameStates::getScreenWidth() const
{
	return SCREEN_WIDTH;
}

int GameStates::getScreenHeight() const
{
	return SCREEN_HEIGHT;
}

float GameStates::getCurPosX() const
{
	return curPosX;
}

float GameStates::getCurPosY() const
{
	return curPosY;
}

bool GameStates::getKey(int key) const
{
	return Keys[key];
}

glm::mat4 GameStates::getViewMatrix() const
{
	return viewMatrix;
}

glm::mat4 GameStates::getProjectionMatrix() const
{
	return projectionMatrix;
}