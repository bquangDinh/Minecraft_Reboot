#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Game.h"

using namespace std;

const int WINDOW_WIDTH = 800;

const int WINDOW_HEIGHT = 600;

const char* WINDOW_TITLE = "Minecraft Reboot";

Game game(WINDOW_WIDTH, WINDOW_HEIGHT);

/* Windows Events */
// Window Resize event
void framebufferSizeCallback(GLFWwindow*, int, int);

void keyCallback(GLFWwindow*, int, int, int, int);

void mousePosCallback(GLFWwindow*, double, double);

int main() {
	// Init GLFW
	// GLFW handles Windows-based functionalities such as creating window, managing states of a window, etc
	glfwInit();

	// Since we would be using OpenGL 3.3
	// We need to tell GLFW so it can arrange configuration accordingly
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Since we would be using core-profile OpenGL
	// We need to tell GLFW about that too
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// On MaC OS only
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a Window object
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	if (window == NULL) {
		cout << "Failed to create GLFW Window" << endl;

		// Free memory
		glfwTerminate();

		return -1;
	}

	// If create window success, make this window the current context
	glfwMakeContextCurrent(window);

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;

		// Free memory
		glfwTerminate();

		return -1;
	}

	// Configure some global OpenGL's states
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	// Set OpenGL's viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Register window's events
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	
	glfwSetKeyCallback(window, keyCallback);

	glfwSetCursorPosCallback(window, mousePosCallback);

	// Make sure the mouse won't move out of the window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize Game
	game.init();

	float deltaTime = 0.0f, lastTime = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		float currentTime = (float)glfwGetTime();

		deltaTime = currentTime - lastTime;

		lastTime = currentTime;

		game.update(deltaTime);

		game.render(deltaTime);

		// Call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	game.destroy();

	glfwTerminate();

	return 0;
}

// Called when the given window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	// resize OpenGL's viewport
	glViewport(0, 0, width, height);
}

// Called when the given window receives user's keyboard input
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// Check if the escape key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		// Set the flag for the window will be close
		glfwSetWindowShouldClose(window, true);

		return;
	}

	game.processKeyInput(key, action);
}

// Called when the mouse moves in the given window
void mousePosCallback(GLFWwindow* window, double x, double y) {
	game.processMouseInput((float)x, (float)y);
}