#pragma once

#include <memory>

#include "GameObject.h"
#include "Camera.h"

class Player: public GameObject
{
private:
	unique_ptr<Camera> playerCamera;
public:
	Player();

	~Player();

	void init() override;

	void update(float) override;

	void render(float) override;

	void destroy() override;
};

