#include "Player.h"

Player::Player(glm::vec3 position, Camera* camera)
	:
	GameObject(position)
{
	this->position = position;
	this->forward_movement = 0;
	this->side_movement = 0;
	this->vertical_movement = 0;
	this->velocity = glm::vec3(0.0f);

	this->camera = camera;

	this->set_collision_shape(new Collision::Box(&(this->position), &(this->velocity), 2.0f, 2.0f, 2.0f));
	this->set_graphic_model(ModelClass::Box, new ModelInstance(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
}

Player::~Player()
{
	delete this->camera;
}

void Player::update(const float dt)
{
	this->camera->update_camera_vectors();

	if (forward_movement || side_movement || vertical_movement) {
		glm::vec3 front_translate = this->camera->get_front() * static_cast<float>(forward_movement);
		glm::vec3 side_translate = this->camera->get_right() * static_cast<float>(side_movement);
		glm::vec3 vertical_translate = this->camera->get_world_up() * static_cast<float>(vertical_movement);
		glm::vec3 translate = glm::vec3(front_translate.x + side_translate.x, vertical_translate.y, front_translate.z + side_translate.z);
		this->velocity = glm::normalize(translate) * this->speed;
	}
	else {
		this->velocity = glm::vec3(0.0f);
	}

	this->position += this->velocity * dt;
	this->model_instance->set_position(this->position);
	this->camera->set_position(this->position + glm::vec3(0.0f, 2.0f, 0.0f));
}

void Player::update_keyboard_input(GLFWwindow* window, int key, int action)
{
	switch (key)
	{
	case GLFW_KEY_W:
		if (action == GLFW_PRESS)
			this->forward_movement = 1;
		else if (action == GLFW_RELEASE)
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				this->forward_movement = -1;
			else
				this->forward_movement = 0;
		break;

	case GLFW_KEY_A:
		if (action == GLFW_PRESS)
			this->side_movement = -1;
		else if (action == GLFW_RELEASE)
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				this->side_movement = 1;
			else
				this->side_movement = 0;
		break;

	case GLFW_KEY_S:
		if (action == GLFW_PRESS)
			this->forward_movement = -1;
		else if (action == GLFW_RELEASE)
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				this->forward_movement = 1;
			else
				this->forward_movement = 0;
		break;
		
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
			this->side_movement = 1;
		else if (action == GLFW_RELEASE)
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				this->side_movement = -1;
			else
				this->side_movement = 0;
		break;

	default:
		break;
	}
}