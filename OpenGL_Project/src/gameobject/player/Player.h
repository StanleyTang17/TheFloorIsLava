#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include"GlobalDebug.h"
#include"engine/camera/Camera.h"
#include"engine/hitbox/Hitbox.h"
#include"engine/input/KeyInput.h"
#include"gameobject/GameObject.h"
#include"libs/NATIVE.h"

class Player : public GameObject, public KeyInput
{
private:
	float speed = 7.0f;
	float jump_speed = 15.0f;

	// MOVEMENT CONTROL
	int front_movement;
	int side_movement;
	bool jump_movement;

	glm::vec3 front;
	glm::vec3 right;

public:
	Player(glm::vec3 position);
	~Player();

	void update_direction(glm::vec3 front, glm::vec3 right);
	void update_velocity(const float dt) override;
	void update();
	void handle_key_input(GLFWwindow* window, int key, int action) override;
	void reset(glm::vec3 position);
	
};

#endif