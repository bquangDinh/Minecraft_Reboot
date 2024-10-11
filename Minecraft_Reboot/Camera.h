#pragma once

#include "GameObject.h"

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class Camera : public GameObject
{
private:
	// Camera direction
	vec3 cameraFront;

	// Camera up vector
	vec3 cameraUp;

	mat4 projectionMatrix;

	const float CAMERA_SPEED;

	const float CAMERA_SENSITIVITY;

	const float FOV;

	// Limit how much the camera can look up or down
	const float PITCH_LIMIT;

	// Look up or down
	float pitch;

	// Look left or right
	float yaw;

	float lastX, lastY;

	bool firstAround;

	bool firstGetProjectionMatrix;

	void processKeyInputs(float);

	/* Moving Functions */
	void moveForward(float);

	void moveBackward(float);

	void moveLeft(float);

	void moveRight(float);

	/* Rotation Functions */
	void turnAround(float, float);
public:
	Camera();

	mat4 getViewMatrix();

	mat4 getProjectionMatrix();

	void init() override;

	void update(float) override;

	void render(float) override;

	void destroy() override;
};

