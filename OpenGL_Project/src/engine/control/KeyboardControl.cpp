#include "KeyboardControl.h"

KeyboardControl::KeyboardControl(int forward_key, int backward_key, int left_key, int right_key, int up_key, int down_key)
{
	this->front_movement = 0;
	this->side_movement = 0;
	this->vertical_movement = 0;

	this->forward_key = forward_key;
	this->backward_key = backward_key;
	this->left_key = left_key;
	this->right_key = right_key;
	this->up_key = up_key;
	this->down_key = down_key;
}

KeyboardControl::KeyboardControl()
{
	this->front_movement = 0;
	this->side_movement = 0;
	this->vertical_movement = 0;

	this->forward_key = GLFW_KEY_W;
	this->backward_key = GLFW_KEY_S;
	this->left_key = GLFW_KEY_A;
	this->right_key = GLFW_KEY_D;
	this->up_key = GLFW_KEY_SPACE;
	this->down_key = GLFW_KEY_LEFT_SHIFT;
}

void KeyboardControl::update_input(GLFWwindow* window, int key, int action)
{
	if (key == this->forward_key)
	{
		if (action == GLFW_PRESS)
			this->front_movement = 1;
		else if (action == GLFW_RELEASE)
		{
			if (glfwGetKey(window, this->backward_key) == GLFW_PRESS)
				this->front_movement = -1;
			else
				this->front_movement = 0;
		}
	}
	else if (key == this->backward_key)
	{
		if (action == GLFW_PRESS)
			this->front_movement = -1;
		else if (action == GLFW_RELEASE)
		{
			if (glfwGetKey(window, this->forward_key) == GLFW_PRESS)
				this->front_movement = 1;
			else
				this->front_movement = 0;
		}
	}
	else if (key == this->right_key)
	{
		if (action == GLFW_PRESS)
			this->side_movement = 1;
		else if (action == GLFW_RELEASE)
		{
			if (glfwGetKey(window, this->left_key) == GLFW_PRESS)
				this->side_movement = -1;
			else
				this->side_movement = 0;
		}
	}
	else if (key == this->left_key)
	{
		if (action == GLFW_PRESS)
			this->side_movement = -1;
		else if (action == GLFW_RELEASE)
		{
			if (glfwGetKey(window, this->right_key) == GLFW_PRESS)
				this->side_movement = 1;
			else
				this->side_movement = 0;
		}
	}
	else if (key == this->up_key)
	{
		if (action == GLFW_PRESS)
			this->vertical_movement = 1;
		else if (action == GLFW_RELEASE)
		{
			if (glfwGetKey(window, this->down_key) == GLFW_PRESS)
				this->vertical_movement = -1;
			else
				this->vertical_movement = 0;
		}
	}
	else if (key == this->down_key)
	{
		if (action == GLFW_PRESS)
			this->vertical_movement = -1;
		else if (action == GLFW_RELEASE)
		{
			if (glfwGetKey(window, this->up_key) == GLFW_PRESS)
				this->vertical_movement = 1;
			else
				this->vertical_movement = 0;
		}
	}

}