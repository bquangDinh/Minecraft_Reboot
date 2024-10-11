#include "GameStates.h"

shared_ptr<GameStates> GameStates::getInstance()
{
	static shared_ptr<GameStates> instance(new GameStates());	// Guaranteed to be destroyed.
																// Instantiated on first use.
	return instance;
}

void GameStates::cleanUp()
{

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