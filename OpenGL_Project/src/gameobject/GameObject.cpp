#include "GameObject.h"

GameObject::GameObject(glm::vec3 position)
{
	this->position = position;
	this->model_instance = nullptr;
	this->hitbox = nullptr;
	this->velocity = glm::vec3(0.0f);
	this->in_air = false;
}

GameObject::~GameObject()
{
	if (model_instance != nullptr)
		delete model_instance;
	if (hitbox != nullptr)
		delete hitbox;
}

void GameObject::apply_gravity(const float dt)
{
	if (this->in_air)
		this->velocity.y += GameObject::GRAVITY * dt;
}

void GameObject::update_velocity(const float dt)
{
	this->apply_gravity(dt);
}

void GameObject::update_hitbox()
{
	if (hitbox != nullptr)
		hitbox->set_center(this->position);
}

void GameObject::set_hitbox(Hitbox* hitbox)
{
	if (this->hitbox != nullptr)
		delete this->hitbox;
	this->hitbox = hitbox;
}

void GameObject::set_graphic_model(ModelInstance* model_instance)
{
	delete this->model_instance;
	this->model_instance = model_instance;
}

void GameObject::move(const float dt)
{
	this->position += this->velocity * dt;
	if (this->model_instance != nullptr)
		this->model_instance->set_position(this->position);
}

void GameObject::get_hitbox_vertices(Face face, glm::vec3 vertices[4])
{
	if (this->hitbox != nullptr)
		this->hitbox->get_vertices(face, vertices);
}

glm::vec3 GameObject::get_hitbox_size()
{
	if (this->hitbox != nullptr)
		return this->hitbox->get_size();
}