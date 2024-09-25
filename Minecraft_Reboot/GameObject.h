#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "GameStates.h"
#include "ShaderManager.h"

class GameObject
{
protected:
	GameStates* gameStates;

	ShaderManager* shaderManager;

	glm::vec3 position;
public:
	GameObject();

	virtual ~GameObject();

	virtual void setPosition(glm::vec3 position);

	virtual void init() = 0;

	virtual void update(float) = 0;

	virtual void render(float) = 0;

	virtual void destroy() = 0;
};

