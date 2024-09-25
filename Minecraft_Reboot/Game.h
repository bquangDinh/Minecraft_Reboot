#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "GameStates.h"
#include "ShaderManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "Quad.h"

using namespace std;

class Game
{
private:
	std::vector<shared_ptr<GameObject>> gameObjects;

	GameStates* gameStates;

	ShaderManager* shaderManager;

	Camera* mainCamera;

	const float FOV;
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

