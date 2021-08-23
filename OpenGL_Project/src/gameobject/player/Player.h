#pragma once

#include"engine/camera/Camera.h"
#include"engine/control/KeyboardControl.h"
#include"gameobject/GameObject.h"
#include"libs/NATIVE.h"

class Player : public GameObject
{
private:
	float speed = 5.0f;
	Camera* camera;
	KeyboardControl* control;

public:
	Player(glm::vec3 position, Camera* camera);
	~Player();

	void update_velocity();
	void update();
	KeyboardControl* get_keyboard_control() { return this->control; }
};