#include "Camera.h"

Camera::Camera(): 
	cameraFront(0.0f, 0.0f, -1.0f),
	cameraPos(0.0f, 0.0f, 3.0f),
	cameraUp(0.0f, 1.0f, 0.0f),
	CAMERA_SPEED(5.0f),
	CAMERA_SENSITIVITY(0.25f),
	PITCH_LIMIT(89.0f),
	pitch(0.0f),
	yaw(-90.0f),
	firstAround(false)
{
	lastX = gameStates->getScreenWidth() / 2.0f;
	lastY = gameStates->getScreenHeight() / 2.0f;
}

void Camera::init() {
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

	view = lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

	return view;
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
	cameraPos += cameraFront * CAMERA_SPEED * deltaTime;
}

void Camera::moveBackward(float deltaTime) {
	cameraPos -= cameraFront * CAMERA_SPEED * deltaTime;
}

void Camera::moveLeft(float deltaTime) {
	cameraPos -= normalize(cross(cameraFront, cameraUp)) * CAMERA_SPEED * deltaTime;
}

void Camera::moveRight(float deltaTime) {
	cameraPos += normalize(cross(cameraFront, cameraUp)) * CAMERA_SPEED * deltaTime;
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