#include "Camera.h"

Camera::Camera(): 
	cameraFront(0.0f, 0.0f, -1.0f),
	cameraUp(0.0f, 1.0f, 0.0f),
	projectionMatrix(mat4(1.0f)),
	CAMERA_SPEED(25.0f),
	CAMERA_SENSITIVITY(0.25f),
	PITCH_LIMIT(89.0f),
	FOV(45.0f),
	pitch(0.0f),
	yaw(-90.0f),
	firstAround(true),
	firstGetProjectionMatrix(true)
{
	lastX = gameStates->getScreenWidth() / 2.0f;
	lastY = gameStates->getScreenHeight() / 2.0f;
}

void Camera::init() {
	// Initialize the camera here
}

void Camera::update(float deltaTime) {
	// Update the camera here
	processKeyInputs(deltaTime);

	turnAround(gameStates->getCurPosX(), gameStates->getCurPosY());
}

void Camera::render(float deltaTime) {
	// Render the camera here
}

void Camera::destroy() {
	// Destroy the camera here
}

mat4 Camera::getViewMatrix() {
	mat4 view = mat4(1.0f);

	view = lookAt(this->transform.position, cameraFront + this->transform.position, cameraUp);

	return view;
}

mat4 Camera::getProjectionMatrix() {
	if (firstGetProjectionMatrix) {
		projectionMatrix = perspective(radians(FOV), (float)gameStates->getScreenWidth() / (float)gameStates->getScreenHeight(), 0.1f, 100.0f);

		firstGetProjectionMatrix = false;
	}
	
	return projectionMatrix;
}

void Camera::processKeyInputs(float deltaTime) {
	if (gameStates->getKey(GLFW_KEY_W)) {
		moveForward(deltaTime);
	}

	if (gameStates->getKey(GLFW_KEY_S)) {

		moveBackward(deltaTime);
	}

	if (gameStates->getKey(GLFW_KEY_A)) {
		moveLeft(deltaTime);
	}

	if (gameStates->getKey(GLFW_KEY_D)) {
		moveRight(deltaTime);
	}
}

void Camera::moveForward(float deltaTime) {
	this->transform.setPosition(this->transform.position + cameraFront * CAMERA_SPEED * deltaTime);
}

void Camera::moveBackward(float deltaTime) {
	this->transform.setPosition(this->transform.position - cameraFront * CAMERA_SPEED * deltaTime);
}

void Camera::moveLeft(float deltaTime) {
	this->transform.setPosition(this->transform.position - normalize(cross(cameraFront, cameraUp)) * CAMERA_SPEED * deltaTime);
}

void Camera::moveRight(float deltaTime) {
	this->transform.setPosition(this->transform.position + normalize(cross(cameraFront, cameraUp)) * CAMERA_SPEED * deltaTime);
}

void Camera::turnAround(float x, float y) {
	if (!firstAround) {
		lastX = x;
		lastY = y;
		firstAround = true;
	}

	// How much the mouse has moved from the last frame
	float xOffset = x - lastX;

	// Since y starts from the top left corner
	float yOffset = lastY - y;

	// Update lastX and lastY
	lastX = x;
	lastY = y;

	xOffset *= CAMERA_SENSITIVITY;
	yOffset *= CAMERA_SENSITIVITY;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > PITCH_LIMIT) {
		pitch = PITCH_LIMIT;
	}

	if (pitch < -PITCH_LIMIT) {
		pitch = -PITCH_LIMIT;
	}

	vec3 front;

	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));

	cameraFront = normalize(front);
}