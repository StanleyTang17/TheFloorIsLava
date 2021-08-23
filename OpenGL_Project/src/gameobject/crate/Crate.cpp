#include"Crate.h"

Crate::Crate(glm::vec3 position, Collision::Behavior behavior, float size)
	:
	GameObject(position)
{
	this->set_graphic_model(ModelClass::Box, new ModelInstance(position, glm::vec3(0.0f), glm::vec3(size)));
	this->set_collision_shape(new Collision::Box(&(this->position), &(this->velocity), behavior, size * 2, size * 2, size * 2));
	this->control = new KeyboardControl(GLFW_KEY_I, GLFW_KEY_K, GLFW_KEY_J, GLFW_KEY_L, GLFW_KEY_P, GLFW_KEY_SEMICOLON);
}

void Crate::update_velocity()
{
	int front_movement = this->control->get_front_movement();
	int side_movement = this->control->get_side_movement();
	int vertical_movement = this->control->get_vertical_movement();

	this->velocity = glm::vec3(front_movement * this->speed, vertical_movement * this->speed, side_movement * this->speed);
}