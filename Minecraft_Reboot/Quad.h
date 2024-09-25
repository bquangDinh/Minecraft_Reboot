#pragma once

#include "glm/glm.hpp"

#include "GameObject.h"

class Quad : public GameObject
{
private:
	GLuint VAO, VBO, EBO, texture;

	shared_ptr<ShaderProgram> shaderProgram;
public:
	Quad();

	void init() override;

	void update(float) override;

	void render(float) override;

	void destroy() override;
};

