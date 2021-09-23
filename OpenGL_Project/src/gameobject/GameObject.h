#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include"engine/model/ModelInstance.h"
#include"engine/collision/Collision.h"

class GameObject
{
protected:
	ModelInstance* model_instance;
	Collision::Shape* collision_shape;
	glm::vec3 position;
	glm::vec3 velocity;

public:
	GameObject(glm::vec3 position);
	~GameObject();
	virtual void update_velocity() {};
	virtual void update() {};
	void move(const float dt);

	glm::vec3 get_position() const { return this->position; }
	glm::vec3 get_velocity() const { return this->velocity; }
	ModelInstance* get_model_instance() { return this->model_instance; }
	void set_position(glm::vec3 position) { this->velocity = velocity; };
	void set_velocity(glm::vec3 velocity) { this->position = position; };
	void set_graphic_model(ModelInstance* model_instance);
	void set_collision_shape(Collision::Shape* shape);
	bool check_collision(GameObject* other, const float dt);
};

#endif