#pragma once

#include"GLM.h"

class ModelInstance
{
protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 model_matrix;

	void update_model_matrix();
public:
	ModelInstance(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void set_position(glm::vec3 position);
	void set_rotation(glm::vec3 rotation);
	void set_scale(glm::vec3 scale);
	void move(glm::vec3 translate);
	void rotate(glm::vec3 rotate);
	void _scale(glm::vec3 scale);
	glm::vec3 get_position() const { return this->position; }
	glm::mat4 get_model_matrix();
};