#include "Game.h"

Game::Game(int screenWidth, int screenHeight):
	gameStates(&GameStates::getInstance()),
	shaderManager(&ShaderManager::getInstance()),
	FOV(45.0f)
{
	// Initialize game states
	gameStates->SCREEN_WIDTH = screenWidth;
	gameStates->SCREEN_HEIGHT = screenHeight;

	mainCamera = new Camera();
}

Game::~Game() {
	gameStates = nullptr;

	shaderManager = nullptr;

	delete mainCamera;
}

void Game::init() {
	// Init Shaders
	shaderManager->loadShaderProgram("main", "vertex_shader.vert", "fragment_shader.frag");

	// Init main camera
	mainCamera->init();

	// Init projection matrix
	gameStates->projectionMatrix = glm::perspective(glm::radians(FOV), (float)gameStates->SCREEN_WIDTH / (float)gameStates->SCREEN_HEIGHT, 0.1f, 100.0f);
	
	// Add game objects
	const shared_ptr<GameObject> quad = make_shared<Quad>();

	quad->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	gameObjects.push_back(quad);

	// Init game objects
	for (auto& gameObject : gameObjects) {
		gameObject->init();
	}
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

	// Render game objects
	for (auto& gameObject : gameObjects) {
		gameObject->render(deltaTime);
	}
}

void Game::destroy() {
	// Destroy the game here and free up memory

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
