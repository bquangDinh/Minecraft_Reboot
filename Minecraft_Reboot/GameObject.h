#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "GameStates.h"
#include "ShaderManager.h"

using namespace std;

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform() :
		position(glm::vec3(0.0f, 0.0f, 0.0f)),
		rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
		scale(glm::vec3(1.0f, 1.0f, 1.0f))
	{}

	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
		position(position),
		rotation(rotation),
		scale(scale)
	{}

	Transform(const Transform& other) :
		position(other.position),
		rotation(other.rotation),
		scale(other.scale)
	{}

	Transform& operator=(const Transform& other) {
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;

		return *this;
	}

	~Transform() {}

	void setPosition(glm::vec3 position) {
		this->position = position;
	}

	void setRotation(glm::vec3 rotation) {
		this->rotation = rotation;
	}

	void setScale(glm::vec3 scale) {
		this->scale = scale;
	}
};

class GameObject
{
protected:
	shared_ptr<GameStates> gameStates;

	shared_ptr<ShaderManager> shaderManager;

	Transform transform;
public:
	GameObject();

	virtual ~GameObject();

	virtual void init() = 0;

	virtual void update(float) = 0;

	virtual void render(float) = 0;

	virtual void destroy() = 0;
};

