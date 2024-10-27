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
	playerCamera->render(deltaTime);
}

void Player::destroy()
{
	playerCamera->destroy();
}