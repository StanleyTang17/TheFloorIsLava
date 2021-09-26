#pragma once

#ifndef MODEL_INSTANCE_H
#define MODEL_INSTANCE_H

#include"AnimatedModel.h"
#include"utility/Utility.h"

class ModelInstance
{
private:
	Model* model;

	Sequence current_animation;
	float animation_time;
	bool stop;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	inline AnimatedModel* get_animated_model() const { return static_cast<AnimatedModel*>(this->model); }

public:
	ModelInstance(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	ModelInstance(std::string loaded_model_name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	void update(const float dt);
	void render(Shader* vertex_shader, Shader* fragment_shader);

	void play_animation(std::string animation_name);
	void pause();
	void unpause();

	inline void set_position(glm::vec3 position) { this->position = position; }
	inline void set_rotation(glm::vec3 rotation) { this->rotation = rotation; }
	inline void set_scale(glm::vec3 scale) { this->scale = scale; }
	inline void move(glm::vec3 translate) { this->position += translate; }
	inline void rotate(glm::vec3 rotate) { this->rotation += rotate; }
	inline void _scale(glm::vec3 scale) { this->scale += scale; }
	inline bool is_animated() { return this->model->is_animated(); }
	glm::vec3 get_position() const { return this->position; }
	glm::mat4 get_model_matrix();
};

#endif