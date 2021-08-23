#pragma once

#include"gameobject/GameObject.h"
#include"engine/control/KeyboardControl.h"

class Ball : public GameObject
{
private:
	KeyboardControl* control;
	float speed = 3.0f;

public:
	Ball(glm::vec3 position);
	void update() {};
	void update_velocity();

	KeyboardControl* get_keyboard_control() { return this->control; };
};
