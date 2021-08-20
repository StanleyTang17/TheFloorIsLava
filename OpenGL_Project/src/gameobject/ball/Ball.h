#pragma once

#include"gameobject/GameObject.h"

class Ball : public GameObject
{
public:
	Ball(glm::vec3 position);
	void update(const float dt);
};
