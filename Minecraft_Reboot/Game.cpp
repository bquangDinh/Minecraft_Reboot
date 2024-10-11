#include "Game.h"

#include "Chunk.h"

Game::Game(int screenWidth, int screenHeight):
	gameStates(&GameStates::getInstance()),
	shaderManager(&ShaderManager::getInstance()),
	textureManager(&TextureManager::getInstance()),
	FOV(45.0f)
{
	// Initialize game states
	gameStates->SCREEN_WIDTH = screenWidth;
	gameStates->SCREEN_HEIGHT = screenHeight;

	mainCamera = make_unique<Camera>();
}

Game::~Game() {
}

void Game::init() {
#ifdef ENABLE_DEPTH_TEST
	glEnable(GL_DEPTH_TEST);
#endif // ENABLE_DEPTH_TEST

#ifdef ENABLE_CULL_FACE
	glEnable(GL_CULL_FACE);
#endif // ENABLE_CULL_FACE

#ifdef WIREFRAME_MODE
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif // WIREFRAME_MODE

	// Init Shaders
	shaderManager->loadShaderProgram(MAIN_SHADER_PROGRAM, "vertex_shader.vert", "fragment_shader.frag");

	// Init Textures
	textureManager->loadTextureArray(TEXTURE_ATLAS, true, true, MAIN_TEXTURE_ARRAY);

	// Init main camera
	mainCamera->init();

	// Init projection matrix
	gameStates->projectionMatrix = glm::perspective(glm::radians(FOV), (float)gameStates->SCREEN_WIDTH / (float)gameStates->SCREEN_HEIGHT, 0.1f, 1000.0f);
	
	// Add game objects
	gameObjects.push_back(make_shared<Chunk>(vec3(0.0f, 0.0f, 0.0f), vec3(120.0f, 120.0f, 120.0f)));

	// Init game objects
	for (auto& gameObject : gameObjects) {
		gameObject->init();
	}

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
	glClearColor(0.059f, 0.204f, 0.376f, 1.0f);
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
	//meshBuilder->render();
}

void Game::destroy() {
	// Destroy the game here and free up memory
	//meshBuilder->cleanUp();

	// Destroy main camera
	mainCamera->destroy();

	for (auto& gameObject : gameObjects) {
		gameObject->destroy();
	}

	gameObjects.clear();

	textureManager->cleanUp();

	shaderManager->cleanUp();

	gameStates->cleanUp();

	gameStates = nullptr;

	shaderManager = nullptr;

	textureManager = nullptr;

	cout << "Destroy Game succeed!" << endl;
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
