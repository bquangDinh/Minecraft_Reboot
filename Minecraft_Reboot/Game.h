#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

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
public:
	Game(int, int);

	~Game();

	void init();

	void update(float);

	void render(float);

	void destroy();

	void processKeyInput(int, int);

	void processMouseInput(float, float);
};

