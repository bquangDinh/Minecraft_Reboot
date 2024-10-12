#include "Player.h"

Player::Player()
{
	transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

	playerCamera = new Camera();
}

Player::~Player()
{
	delete playerCamera;
}

void Player::init()
{
	playerCamera->init();
}

void Player::update(float deltaTime)
{
	playerCamera->update(deltaTime);
}

void Player::render(float deltaTime)
{
	shaderManager->getShaderProgram(MAIN_SHADER_PROGRAM)->SetMatrix4("view", playerCamera->getViewMatrix());

	shaderManager->getShaderProgram(MAIN_SHADER_PROGRAM)->SetMatrix4("projection", playerCamera->getProjectionMatrix());
}

void Player::destroy()
{
	playerCamera->destroy();
}