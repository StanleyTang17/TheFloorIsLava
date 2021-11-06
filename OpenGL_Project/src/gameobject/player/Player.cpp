#include "Player.h"

glm::vec3 global::position = glm::vec3(0.0f);
glm::vec3 global::velocity = glm::vec3(0.0f);
glm::vec3 global::size = glm::vec3(0.0f);

Player::Player(glm::vec3 position)
	:
	GameObject(position)
{
	this->position = position;
	this->velocity = glm::vec3(0.0f);

	this->set_hitbox(new Hitbox(this->position, glm::vec3(1.0f, 2.0f, 1.0f)));

	this->front_movement = 0;
	this->side_movement = 0;
	this->in_air = false;

	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->right = glm::vec3(1.0f, 0.0f, 0.0f);

	global::size = this->hitbox->get_size();
}

Player::~Player()
{

}

void Player::update_direction(glm::vec3 front, glm::vec3 right)
{
	this->front = front;
	this->right = right;
}

void Player::update_velocity(const float dt)
{
	if (front_movement || side_movement)
	{
		glm::vec3 front_translate = this->front * static_cast<float>(front_movement);
		glm::vec3 side_translate = this->right * static_cast<float>(side_movement);

		glm::vec3 translate = glm::vec3(front_translate.x + side_translate.x, 0, front_translate.z + side_translate.z);
		translate = glm::normalize(translate);

		this->velocity.x = translate.x * this->speed;
		this->velocity.z = translate.z * this->speed;
	}
	else
	{
		this->velocity.x = 0;
		this->velocity.z = 0;
	}

	if (jump_movement && !this->in_air)
	{
		this->velocity.y = this->jump_speed;
		this->in_air = true;
	}
	
	this->apply_gravity(dt);
}

void Player::update()
{
	global::position = this->position;
	global::velocity = this->velocity;
}

void Player::handle_key_input(GLFWwindow* window, int key, int action)
{
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS)
			this->front_movement = 1;
		else if (action == GLFW_RELEASE)
		{
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				this->front_movement = -1;
			else
				this->front_movement = 0;
		}
	}
	else if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS)
			this->front_movement = -1;
		else if (action == GLFW_RELEASE)
		{
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				this->front_movement = 1;
			else
				this->front_movement = 0;
		}
	}
	else if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
			this->side_movement = 1;
		else if (action == GLFW_RELEASE)
		{
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				this->side_movement = -1;
			else
				this->side_movement = 0;
		}
	}
	else if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS)
			this->side_movement = -1;
		else if (action == GLFW_RELEASE)
		{
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				this->side_movement = 1;
			else
				this->side_movement = 0;
		}
	}
	else if (key == GLFW_KEY_SPACE)
	{
		if (action == GLFW_PRESS)
			this->jump_movement = true;
		else if (action == GLFW_RELEASE)
			this->jump_movement = false;
	}
	else if (key == GLFW_KEY_LEFT_SHIFT)
	{
		if (action == GLFW_PRESS)
			this->speed = 2.0f;
		else if (action == GLFW_RELEASE)
			this->speed = 5.0f;
	}
}