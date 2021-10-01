#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include"engine/camera/Camera.h"
#include"engine/control/KeyboardControl.h"
#include"gameobject/firearm/Firearm.h"
#include"libs/NATIVE.h"

class Player : public GameObject
{
private:
	float speed = 5.0f;
	Camera* camera;
	KeyboardControl* control;
	Firearm* firearm;

public:
	Player(glm::vec3 position, Camera* camera);
	~Player();

	void update_velocity();
	void update();
	void update_mouse_input(GLFWwindow* window, int button, int action);
	void update_keyboard_input(GLFWwindow* window, int key, int action);
	void equip(Firearm* firearm);

	inline Firearm* get_firearm() { return this->firearm; }
};

#endif