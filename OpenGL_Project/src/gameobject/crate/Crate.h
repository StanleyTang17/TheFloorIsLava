#ifndef CRATE_H
#define CRATE_H

#include"gameobject/GameObject.h"
#include"engine/control/KeyboardControl.h"
#include"engine/hitbox/Hitbox.h"

class Crate : public GameObject
{
private:
	KeyboardControl* control;
	float speed = 3.0f;
	Hitbox hitbox;

public:
	Crate(glm::vec3 position, float size);
	void update_velocity();
	void set_control(KeyboardControl* control) { this->control = control; }
	Hitbox get_hitbox() { return this->hitbox; }

	KeyboardControl* get_keyboard_control() { return this->control; };
};

#endif