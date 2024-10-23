#include "Game.h"

#include "ChunkManager.h"

Game::Game(int screenWidth, int screenHeight):
	gameStates(GameStates::getInstance()),
	shaderManager(ShaderManager::getInstance()),
	textureManager(TextureManager::getInstance())
{
	// Initialize game states
	gameStates->SCREEN_WIDTH = screenWidth;
	gameStates->SCREEN_HEIGHT = screenHeight;
}

Game::~Game() {
}

void Game::init(GLFWwindow* window) {
#ifdef ENABLE_DEPTH_TEST
	glEnable(GL_DEPTH_TEST);
#endif // ENABLE_DEPTH_TEST

#ifdef ENABLE_CULL_FACE
	glEnable(GL_CULL_FACE);
#endif // ENABLE_CULL_FACE

#ifdef WIREFRAME_MODE
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif // WIREFRAME_MODE

	this->window = window;

	// Init Shaders
	shaderManager->loadShaderProgram(MAIN_SHADER_PROGRAM, "vertex_shader.vert", "fragment_shader.frag");

	// Init Textures
	textureManager->loadTextureArray(TEXTURE_ATLAS, true, true, MAIN_TEXTURE_ARRAY);

	// Add game objects
	shared_ptr<Player> player = make_shared <Player>();

	// TODO: better way to pass player to chunk manager
	shared_ptr<ChunkManager> chunkManager = make_shared<ChunkManager>(player);

	gameObjects.push_back(player);

	gameObjects.push_back(chunkManager);

	//gameObjects.push_back(make_shared<Chunk>(vec3(0.0f, 0.0f, 0.0f), vec3(120.0f, 120.0f, 120.0f)));

	// Init game objects
	for (auto& gameObject : gameObjects) {
		gameObject->init();
	}

	cout << "Init Game succeed!" << endl;
}

void Game::run() {
	std::thread updatePlayerThread(&Game::updatePlayer, this);

	std::thread updateThread(&Game::update, this);

	// OpenGL rendering will be on main thread
	render();

	// Done threading
	updateThread.join();

	updatePlayerThread.join();
}

void Game::updatePlayer() {
	while (isRunning) {
		// Get the first game object which is the player
		auto player = gameObjects[0];

		player->update(0.016f);

		// Sleep for 16ms
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}

void Game::update() {
	while (isRunning) {
		// Update everything else except the first game object which is the player
		for (int i = 1; i < gameObjects.size(); i++) {
			gameObjects[i]->update(0.016f);
		}

		// Sleep for 16ms
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}

void Game::render() {
	float deltaTime = 0.0f, lastTime = 0.0f;

	while (isRunning) {
		float currentTime = (float)glfwGetTime();

		deltaTime = currentTime - lastTime;

		lastTime = currentTime;

		// Render background color
		glClearColor(0.059f, 0.204f, 0.376f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render game objects
		for (auto& gameObject : gameObjects) {
			gameObject->render(deltaTime);
		}

		// Call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwWindowShouldClose(window)) {
			isRunning = false;
		}
	}
}

void Game::destroy() {
	// Destroy the game here and free up memory
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
