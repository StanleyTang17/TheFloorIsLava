#include "Firearm.h"

Firearm::Firearm(ModelInstance* graphics_instance, glm::vec3 position, FiringMode mode, int rate_of_fire, float damage, int max_ammo, int max_reserve_ammo)
	:
	GameObject(position), MAX_AMMO(max_ammo), MAX_RESERVE_AMMO(max_reserve_ammo)
{
	this->firing_mode = mode;
	this->rate_of_fire = rate_of_fire;
	this->damage = damage;
	this->ammo = max_ammo;
	this->reserve_ammo = max_reserve_ammo;
	this->time_last_fired = 0;
	this->set_graphic_model(graphics_instance);
	this->parent = nullptr;
	this->offset_from_parent = glm::mat4(1.0f);
	assert(graphics_instance->is_animated());
}

void Firearm::reload()
{
	if (this->ammo < this->MAX_AMMO && this->reserve_ammo > 0)
	{
		int reload_amount = std::min(this->MAX_AMMO, this->reserve_ammo);
		ammo += reload_amount;
		reserve_ammo -= reload_amount;
		this->model_instance->play_animation("reload");
	}
}

void Firearm::fire()
{
	if (this->ammo > 0)
	{
		this->ammo--;
		this->model_instance->play_animation("reload");
	}
	else
		this->reload();
}

void Firearm::update_keyboard_input(GLFWwindow* window, int key, int action)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		this->reload();
}

void Firearm::update_mouse_input(GLFWwindow* window, int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		float current_time = glfwGetTime();
		if (current_time >= this->time_last_fired + 3600 / this->rate_of_fire)
		{
			this->time_last_fired = current_time;
			this->fire();
		}
	}
}

void Firearm::update()
{
	if (this->parent != nullptr)
	{
		//glm::mat4 parent_transform = this->parent->get_model_matrix();
		//glm::vec3 translate = glm::vec3(-1.0f, 1.0f, 0.0f);
		//glm::vec4 pos = glm::vec4(this->parent->get_position() + translate, 1.0f) * this->offset_from_parent;
		//this->position = glm::vec3(pos.x, pos.y, pos.z);
		//this->model_instance->set_rotation(glm::vec3(0.0f, 5.0f, 0.0f));
	}
}