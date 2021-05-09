#include"ModelInstance.h"

ModelInstance::ModelInstance(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

void ModelInstance::set_position(glm::vec3 position)
{
	this->position = position;
}

void ModelInstance::set_rotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void ModelInstance::set_scale(glm::vec3 scale)
{
	this->scale = scale;
}

void ModelInstance::move(glm::vec3 translate)
{
	this->position += translate;
}

void ModelInstance::rotate(glm::vec3 rotate)
{
	this->rotation += rotate;
}

void ModelInstance::_scale(glm::vec3 scale)
{
	this->scale *= scale;
}

void ModelInstance::update_model_matrix()
{
	this->model_matrix = glm::mat4(1.0f);
	this->model_matrix = glm::translate(this->model_matrix, this->position);
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	this->model_matrix = glm::scale(this->model_matrix, this->scale);
}

glm::mat4 ModelInstance::get_model_matrix()
{
	this->update_model_matrix();
	return this->model_matrix;
}