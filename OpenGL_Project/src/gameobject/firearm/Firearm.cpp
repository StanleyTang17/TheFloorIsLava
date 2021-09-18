#include "Firearm.h"

Firearm::Firearm(glm::vec3 position, FiringMode mode, int rate_of_fire, float damage, int max_ammo, int max_reserve_ammo)
	:
	GameObject(position), MAX_AMMO(max_ammo), MAX_RESERVE_AMMO(max_reserve_ammo)
{
	this->firing_mode = mode;
	this->rate_of_fire = rate_of_fire;
	this->damage = damage;
	this->ammo = max_ammo;
	this->reserve_ammo = max_reserve_ammo;
	this->time_last_fired = 0;
}

void Firearm::reload()
{
	if (this->ammo < this->MAX_AMMO && this->reserve_ammo > 0)
	{
		int reload_amount = std::min(this->MAX_AMMO, this->reserve_ammo);
		ammo += reload_amount;
		reserve_ammo -= reload_amount;
	}
}

void Firearm::fire()
{
	if (this->ammo > 0)
		this->ammo--;
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