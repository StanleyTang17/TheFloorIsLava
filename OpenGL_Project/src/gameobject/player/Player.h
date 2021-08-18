#pragma once

#include"engine/camera/Camera.h"
#include"gameobject/GameObject.h"
#include"libs/NATIVE.h"

class Player : public GameObject
{
private:
	// MOVEMENT
	int forward_movement;
	int side_movement;
	int vertical_movement;
	float speed = 5.0f;

	Camera* camera;

public:
	Player(glm::vec3 position, Camera* camera);
	~Player();

	void update_keyboard_input(GLFWwindow* window, int key, int action);
	void update(const float dt);
};