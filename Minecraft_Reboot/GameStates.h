#pragma once

class Game;

#include "glm/glm.hpp"

class GameStates
{
public:
	int getScreenWidth() const;

	int getScreenHeight() const;

	float getCurPosX() const;

	float getCurPosY() const;

	bool getKey(int) const;

	glm::mat4 getViewMatrix() const;

	glm::mat4 getProjectionMatrix() const;
	
	static GameStates& getInstance();
private:
	// Prevent other classes from instantiating this class
	GameStates() {}

	bool Keys[1024] = { false };

	float curPosX = 0, curPosY = 0;

	int SCREEN_WIDTH = 0, SCREEN_HEIGHT = 0;

	glm::mat4 viewMatrix, projectionMatrix;

	friend Game;
};

