#include "ModelInstance.h"

ModelInstance::ModelInstance(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->model = model;
	this->stop = false;
	this->repeat = false;
	this->animation_time = 0;

	if (this->model->is_animated())
	{
		this->current_animation = this->get_animated_model()->get_animations().begin()->second;
		this->animation_time = this->current_animation.start_time;
	}
	else
	{
		this->current_animation = { 0, 0 };
	}
}

ModelInstance::ModelInstance(std::string loaded_model_name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->model = Model::get(loaded_model_name);
	this->stop = false;
	this->repeat = false;
	this->animation_time = 0;

	if (this->model->is_animated())
	{
		this->current_animation = this->get_animated_model()->get_animations().begin()->second;
		this->animation_time = this->current_animation.start_time;
	}
	else
	{
		this->current_animation = { 0, 0 };
	}
}

void ModelInstance::update(const float dt)
{
	if (this->model->is_animated() && !this->stop)
	{
		this->animation_time += dt;

		if (this->animation_time > this->current_animation.end_time)
		{
			if (repeat)
			{
				float duration = this->current_animation.end_time - this->current_animation.start_time;
				while (this->animation_time > this->current_animation.end_time)
					this->animation_time -= duration;
			}
			else
			{
				this->animation_time = this->current_animation.start_time;
				this->stop = true;
			}
		}
	}
}

void ModelInstance::render(Shader* vertex_shader, Shader* fragment_shader)
{
	if (this->model->is_animated())
	{
		std::vector<glm::mat4> bone_matrices = this->get_animated_model()->get_bone_matrices(this->animation_time);
		for (std::size_t j = 0; j < 100; ++j)
			vertex_shader->set_mat_4fv(bone_matrices[j], ("final_bone_matrices[" + std::to_string(j) + "]").c_str(), GL_FALSE);
	}

	vertex_shader->set_mat_4fv(this->get_model_matrix(), "model_matrix", GL_FALSE);

	this->model->render(vertex_shader, fragment_shader);
}

void ModelInstance::play_animation(std::string animation_name, bool repeat)
{
	if (!this->model->is_animated()) return;

	this->current_animation = this->get_animated_model()->get_animations().at(animation_name);
	this->animation_time = this->current_animation.start_time;
	this->repeat = repeat;
}

void ModelInstance::pause()
{
	this->stop = true;
}

void ModelInstance::unpause()
{
	this->stop = false;
}

glm::mat4 ModelInstance::get_model_matrix()
{
	return Utility::generate_transform(this->position, this->rotation, this->scale);
}