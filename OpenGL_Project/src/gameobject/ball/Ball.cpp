#include "Ball.h"

Ball::Ball(glm::vec3 position)
	:
	GameObject(position)
{
	this->set_collision_shape(new Collision::Sphere(&this->position, &this->velocity, 2.0f));
	this->set_graphic_model(ModelClass::Ball, new ModelInstance(position, glm::vec3(0.0f), glm::vec3(1.0f)));
}

void Ball::update(const float dt)
{
	this->get_model_instance()->set_position(this->position);
}