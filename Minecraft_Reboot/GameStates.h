#pragma once

#include "glm/glm.hpp"

#include <memory>

using namespace std;

class GameStates
{
public:
	int getScreenWidth() const;

	int getScreenHeight() const;

	float getCurPosX() const;

	float getCurPosY() const;

	bool getKey(int) const;

	glm::mat4* getViewMatrix() const;

	glm::mat4* getProjectionMatrix() const;
	
	static shared_ptr<GameStates> getInstance();

	void cleanUp();

	glm::mat4* viewMatrix;

	glm::mat4* projectionMatrix;

	bool Keys[1024] = { false };

	float curPosX = 0, curPosY = 0;

	int SCREEN_WIDTH = 0, SCREEN_HEIGHT = 0;
private:
	// Prevent other classes from instantiating this class
	GameStates() {}
};

