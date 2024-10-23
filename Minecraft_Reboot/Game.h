#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>

#include "GameStates.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#include "GameObject.h"

#define TEXTURE_ATLAS "texture_atlas2.png"

using namespace std;

class Game
{
private:
	std::vector<shared_ptr<GameObject>> gameObjects;

	shared_ptr<GameStates> gameStates;

	shared_ptr<ShaderManager> shaderManager;

	shared_ptr<TextureManager> textureManager;

	GLFWwindow** window;

	std::atomic<bool> isRunning{ true };

	void updatePlayer();

	void update();

	void render();
public:
	Game(int, int);

	~Game();

	void init(GLFWwindow** window);

	void destroy();

	void run();

	void processKeyInput(int, int);

	void processMouseInput(float, float);
};

