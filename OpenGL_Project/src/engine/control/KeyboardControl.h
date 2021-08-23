#pragma once

#include"libs/OPENGL.h"

class KeyboardControl
{
private:
	// MOVEMENT
	int front_movement;
	int side_movement;
	int vertical_movement;

	// KEYS
	int forward_key;
	int backward_key;
	int left_key;
	int right_key;
	int up_key;
	int down_key;

public:
	KeyboardControl(int forward_key, int backward_key, int left_key, int right_key, int up_key, int down_key);
	KeyboardControl();

	void update_input(GLFWwindow* window, int key, int action);

	int get_front_movement() const { return this->front_movement; }
	int get_side_movement() const { return this->side_movement; }
	int get_vertical_movement() const { return this->vertical_movement; }
};

