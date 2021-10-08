#include "Firearm.h"

Firearm::Firearm(ModelInstance* graphics_instance, glm::vec3 position, FiringMode mode, int rate_of_fire, float damage, int max_ammo, int max_reserve_ammo)
	:
	GameObject(position), MAX_AMMO(max_ammo), MAX_RESERVE_AMMO(max_reserve_ammo), bullet_ray(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f))
{
	this->firing_mode = mode;
	this->rate_of_fire = rate_of_fire;
	this->damage = damage;
	this->ammo = max_ammo;
	this->reserve_ammo = max_reserve_ammo;
	this->time_last_fired = 0;
	this->set_graphic_model(graphics_instance);
	this->trigger_down = false;
	this->camera = nullptr;
	assert(graphics_instance->is_animated());
}

void Firearm::reload()
{
	if (this->ammo < this->MAX_AMMO && this->reserve_ammo > 0)
	{
		int ammo_missing = this->MAX_AMMO - this->ammo;
		int reload_amount = std::min(ammo_missing, this->reserve_ammo);
		this->ammo += reload_amount;
		this->reserve_ammo -= reload_amount;
		this->model_instance->play_animation("reload");
	}
}

void Firearm::fire()
{
	float current_time = glfwGetTime();
	if (current_time >= this->time_last_fired + 60.0f / this->rate_of_fire &&
		!(this->model_instance->get_animation().name == "reload" && !this->model_instance->is_paused()))
	{
		this->time_last_fired = current_time;
		if (this->ammo > 0)
		{
			this->ammo--;
			this->model_instance->play_animation("fire");
			
		}
		else
			this->reload();
	}
}

void Firearm::update_keyboard_input(GLFWwindow* window, int key, int action)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		this->reload();
}

void Firearm::update_mouse_input(GLFWwindow* window, int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			if (this->firing_mode == FiringMode::SEMI_AUTOMATIC)
				this->fire();
			this->trigger_down = true;
		}
		else if (action == GLFW_RELEASE)
			this->trigger_down = false;
	}
}

void Firearm::update()
{
	if (this->trigger_down && this->firing_mode == FiringMode::AUTOMATIC)
		this->fire();
}

bool Firearm::is_firing() {
	Sequence animation = this->model_instance->get_animation();
	return animation.name == "fire" && !this->model_instance->is_paused();
}

void Firearm::render_bullet(Shader* fragment_shader)
{
	float current_time = glfwGetTime();
	Sequence anim = this->model_instance->get_animation();
	if (!this->model_instance->is_paused() && anim.name == "fire" && current_time < this->time_last_fired + 10.0f / this->rate_of_fire)
	{
		this->camera->update_camera_vectors();
		this->bullet_ray.set_position(
			this->camera->get_position() + this->camera->get_front() * 0.07f
			+ this->camera->get_right() * 0.045f
			+ this->camera->get_camera_up() * -0.012f,

			this->camera->get_position() + this->camera->get_front() * 5.0f
		);
		this->bullet_ray.render(fragment_shader);
	}
}