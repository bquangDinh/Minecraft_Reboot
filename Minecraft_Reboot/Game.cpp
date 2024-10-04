#include "Game.h"

Game::Game(int screenWidth, int screenHeight):
	gameStates(&GameStates::getInstance()),
	shaderManager(&ShaderManager::getInstance()),
	textureManager(&TextureManager::getInstance()),
	FOV(45.0f)
{
	// Initialize game states
	gameStates->SCREEN_WIDTH = screenWidth;
	gameStates->SCREEN_HEIGHT = screenHeight;

	mainCamera = new Camera();
}

Game::~Game() {
	textureManager->cleanUp();

	shaderManager->cleanUp();

	gameStates->cleanUp();

	gameStates = nullptr;

	shaderManager = nullptr;

	textureManager = nullptr;

	delete mainCamera;

	delete meshBuilder;
}

void Game::init() {
	// Init Shaders
	shaderManager->loadShaderProgram(MAIN_SHADER_PROGRAM, "vertex_shader.vert", "fragment_shader.frag");

	// Init Textures
	textureManager->loadTextureArray(TEXTURE_ATLAS, true, MAIN_TEXTURE_ARRAY);

	// Init main camera
	mainCamera->init();

	// Init projection matrix
	gameStates->projectionMatrix = glm::perspective(glm::radians(FOV), (float)gameStates->SCREEN_WIDTH / (float)gameStates->SCREEN_HEIGHT, 0.1f, 100.0f);
	
	// Add game objects

	// Init game objects
	for (auto& gameObject : gameObjects) {
		gameObject->init();
	}

	// Test mesh builder
	// Generate a cube with 6 quads
	// Front
	Quad q1(
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f),
		VOXEL_TYPE::GRASS,
		SIDES::FRONT,
		false
	);

	// Left
	Quad q2(
		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		VOXEL_TYPE::GRASS,
		SIDES::LEFT,
		false
	);

	meshBuilder = new MeshBuilder();

	meshBuilder->addQuad(q1, 1.0f, 1.0f, false);

	meshBuilder->addQuad(q2, 1.0f, 1.0f, false);

	meshBuilder->generateVBO();

	cout << "Init Game succeed!" << endl;
}

void Game::update(float deltaTime) {
	// Update the game here
	mainCamera->update(deltaTime);

	// Update view matrix
	gameStates->viewMatrix = mainCamera->getViewMatrix();

	for (auto& gameObject : gameObjects) {
		gameObject->update(deltaTime);
	}
}

void Game::render(float deltaTime) {
	// Render the game here
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Main camera does not need to render
	// mainCamera.render(deltaTime);

	shaderManager->getShaderProgram(MAIN_SHADER_PROGRAM)->SetMatrix4("view", gameStates->viewMatrix);

	shaderManager->getShaderProgram(MAIN_SHADER_PROGRAM)->SetMatrix4("projection", gameStates->projectionMatrix);

	// Render game objects
	for (auto& gameObject : gameObjects) {
		gameObject->render(deltaTime);
	}

	// Test mesh builder
	meshBuilder->render();
}

void Game::destroy() {
	// Destroy the game here and free up memory
	meshBuilder->cleanUp();

	// Destroy main camera
	mainCamera->destroy();

	for (auto& gameObject : gameObjects) {
		gameObject->destroy();
	}

	gameObjects.clear();
}

void Game::processKeyInput(int key, int action) {
	if (action == GLFW_PRESS) {
		gameStates->Keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		gameStates->Keys[key] = false;
	}
}

void Game::processMouseInput(float x, float y) {
	gameStates->curPosX = x;
	gameStates->curPosY = y;
}
