#pragma once

#include"engine/model/Model.h"
#include"engine/collision/Collision.h"

class GameObject
{
protected:
	ModelClass model_class;
	ModelInstance* model_instance;

	Collision::Shape* collision_shape;

	glm::vec3 position;
	glm::vec3 velocity;

public:
	GameObject(glm::vec3 position);
	~GameObject();
	virtual void update_velocity() = 0;
	virtual void update() = 0;
	void move(const float dt);

	glm::vec3 get_position() const { return this->position; }
	glm::vec3 get_velocity() const { return this->velocity; }
	ModelClass get_model_class() const { return this->model_class; }
	ModelInstance* get_model_instance() const { return model_instance; }
	void set_position(glm::vec3 position) { this->velocity = velocity; };
	void set_velocity(glm::vec3 velocity) { this->position = position; };
	void set_graphic_model(ModelClass model_class, ModelInstance* model_instance);
	void set_collision_shape(Collision::Shape* shape);
	bool check_collision(GameObject* other, const float dt);
};