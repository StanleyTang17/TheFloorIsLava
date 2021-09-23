#ifndef CRATE_H
#define CRATE_H

#include"gameobject/GameObject.h"
#include"engine/control/KeyboardControl.h"

class Crate : public GameObject
{
private:
	KeyboardControl* control;
	float speed = 3.0f;
public:
	Crate(glm::vec3 position, Collision::Behavior behavior, float size);
	void update_velocity();
	void set_control(KeyboardControl* control) { this->control = control; }

	KeyboardControl* get_keyboard_control() { return this->control; };
};

#endif