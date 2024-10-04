#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "GameStates.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#include "GameObject.h"
#include "Camera.h"
#include "MeshBuilder.h"
#include "Quad.h"

#define TEXTURE_ATLAS "texture_atlas.png"

using namespace std;

class Game
{
private:
	std::vector<shared_ptr<GameObject>> gameObjects;

	GameStates* gameStates;

	ShaderManager* shaderManager;

	TextureManager* textureManager;

	Camera* mainCamera;
		
	MeshBuilder* meshBuilder;

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

