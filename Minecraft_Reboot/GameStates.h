#pragma once

#include "glm/glm.hpp"

#include <memory>

#include <atomic>

using namespace std;

class GameStates
{
public:
	int getScreenWidth() const;

	int getScreenHeight() const;

	float getCurPosX() const;

	float getCurPosY() const;

	bool getKey(int) const;
	
	static shared_ptr<GameStates> getInstance();

	void cleanUp();

	bool Keys[1024] = { false };

	float curPosX = 0, curPosY = 0;

	int SCREEN_WIDTH = 0, SCREEN_HEIGHT = 0;

	atomic<bool> isRunning = true;
private:
	// Prevent other classes from instantiating this class
	GameStates() {}
};

