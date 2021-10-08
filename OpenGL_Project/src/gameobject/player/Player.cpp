#include "Player.h"

Player::Player(glm::vec3 position, Camera* camera)
	:
	GameObject(position)
{
	this->position = position;
	this->control = new KeyboardControl();
	this->velocity = glm::vec3(0.0f);

	this->firearm = nullptr;

	this->camera = camera;

	this->set_collision_shape(new Collision::Box(&(this->position), &(this->velocity), Collision::Behavior::KINETIC, 1.0f, 1.0f, 1.0f));
	this->set_graphic_model(new ModelInstance("container", "static", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f)));
}

Player::~Player()
{
	delete this->camera;
}

void Player::update_velocity()
{
	this->camera->update_camera_vectors();

	int front_movement = this->control->get_front_movement();
	int side_movement = this->control->get_side_movement();
	int vertical_movement = this->control->get_vertical_movement();
	if (front_movement || side_movement || vertical_movement) {
		glm::vec3 front_translate = this->camera->get_front() * static_cast<float>(front_movement);
		glm::vec3 side_translate = this->camera->get_right() * static_cast<float>(side_movement);
		glm::vec3 vertical_translate = this->camera->get_world_up() * static_cast<float>(vertical_movement);
		glm::vec3 translate = glm::vec3(front_translate.x + side_translate.x, vertical_translate.y, front_translate.z + side_translate.z);
		this->velocity = glm::normalize(translate) * this->speed;
	}
	else {
		this->velocity = glm::vec3(0.0f);
	}
}

void Player::update()
{
	this->camera->set_position(this->position + glm::vec3(0.0f, 2.0f, 0.0f));
	this->firearm->update();
}

void Player::update_mouse_input(GLFWwindow* window, int button, int action)
{
	if (this->firearm != nullptr)
		this->firearm->update_mouse_input(window, button, action);
}

void Player::update_keyboard_input(GLFWwindow* window, int key, int action)
{
	if (this->firearm != nullptr)
		this->firearm->update_keyboard_input(window, key, action);
	this->control->update_input(window, key, action);
}

void Player::equip(Firearm* firearm)
{
	this->firearm = firearm;
	this->firearm->attach_camera(this->camera);
}