#include "Ball.h"

Ball::Ball(glm::vec3 position)
	:
	GameObject(position)
{
	this->set_collision_shape(new Collision::Sphere(&this->position, &this->velocity, Collision::Behavior::KINETIC, 1.0f));
	this->set_graphic_model(new ModelInstance("ball", "static", position, glm::vec3(0.0f), glm::vec3(1.0f)));
	this->control = new KeyboardControl(GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_ENTER, GLFW_KEY_RIGHT_SHIFT);
}

void Ball::update_velocity()
{
	int front_movement = this->control->get_front_movement();
	int side_movement = this->control->get_side_movement();
	int vertical_movement = this->control->get_vertical_movement();
	
	this->velocity = glm::vec3(front_movement * this->speed, vertical_movement * this->speed, side_movement * this->speed);
}