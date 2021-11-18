#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include"engine/model/ModelInstance.h"
#include"engine/hitbox/Hitbox.h"

class GameObject
{
protected:
	ModelInstance* model_instance;
	Hitbox* hitbox;
	glm::vec3 position;
	glm::vec3 velocity;
	bool in_air;

	void apply_gravity(const float dt);

public:
	const float GRAVITY = -38.4f;

	GameObject(glm::vec3 position);
	~GameObject();
	virtual void update_velocity(const float dt);
	virtual void update() {};
	void update_hitbox();
	void move(const float dt);

	inline glm::vec3 get_position() const { return this->position; }
	inline glm::vec3 get_velocity() const { return this->velocity; }
	inline bool is_in_air() const { return this->in_air; }
	inline ModelInstance* get_model_instance() { return this->model_instance; }
	inline void set_position(glm::vec3 position) { this->position = position; }
	inline void set_velocity(glm::vec3 velocity) { this->velocity = velocity; }
	inline void set_in_air(bool in_air) { this->in_air = in_air; }

	void set_graphic_model(ModelInstance* model_instance);
	void set_hitbox(Hitbox* hitbox);

	void get_hitbox_vertices(Face face, glm::vec3 vertices[4]);
	glm::vec3 get_hitbox_size();
};

#endif