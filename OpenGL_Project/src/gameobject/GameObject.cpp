#include "GameObject.h"

GameObject::GameObject(glm::vec3 position)
{
	this->position = position;
	this->model_class = ModelClass::Undefined;
	this->model_instance = nullptr;
	this->collision_shape = nullptr;
	this->velocity = glm::vec3(0.0f);
}

GameObject::~GameObject()
{
	if (model_instance != nullptr)
		delete model_instance;
	if (collision_shape != nullptr)
		delete collision_shape;
}

void GameObject::set_collision_shape(Collision::Shape* shape)
{
	if (shape == nullptr)
	{
		delete this->collision_shape;
		this->collision_shape = nullptr;
	}
	else
	{
		this->collision_shape = shape;
	}
}

void GameObject::set_graphic_model(ModelClass model_class, ModelInstance* model_instance)
{
	this->model_class = model_class;
	delete this->model_instance;
	this->model_instance = model_instance;
}

bool GameObject::check_collision(GameObject* other) {
	bool hit = false;
	if (this->collision_shape != nullptr && other->collision_shape != nullptr)
		hit = Collision::check(this->collision_shape, other->collision_shape);
	return hit;
}