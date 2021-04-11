#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 world_up)
{
	this->view_matrix = glm::mat4(1.0f);

	this->movement_speed = 1.5f;
	this->sensitivity = 50.0f;
	
	this->position = position;
	this->front = direction;
	this->world_up = world_up;

	this->right = glm::vec3(0.0f);
	this->up = world_up;

	this->pitch = 0.0f;
	this->yaw = -90.0f;
	this->roll = 0.0f;

	this->update_camera_vectors();
}

Camera::~Camera()
{

}

void Camera::update_camera_vectors()
{
	this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front.y = sin(glm::radians(this->pitch));
	this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

	this->front = glm::normalize(this->front);
	this->right = glm::normalize(glm::cross(this->front, this->world_up));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::update_keyboard_input(const float& dt, const int forward_movement, const int side_movement)
{
	if (forward_movement || side_movement) {
		
		glm::vec3 front_translate = this->front * static_cast<float>(forward_movement);
		glm::vec3 side_translate = this->right * static_cast<float>(side_movement);
		glm::vec3 translate = glm::vec3(front_translate.x + side_translate.x, 0.0f, front_translate.z + side_translate.z);
		translate = glm::normalize(translate) * this->movement_speed * dt;
		this->position += translate;
	}
}

void Camera::update_mouse_input(const float& dt, const double& offset_x, const double& offset_y)
{
	this->pitch += static_cast<GLfloat>(offset_y) * this->sensitivity * dt;
	this->yaw += static_cast<GLfloat>(offset_x) * this->sensitivity * dt;

	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	else if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	if (this->yaw > 360.0f || this->yaw < -360.0f)
		this->yaw = 0.0f;
}

const glm::mat4 Camera::get_view_matrix()
{
	this->update_camera_vectors();
	this->view_matrix = glm::lookAt(this->position, this->position + this->front, this->up);
	return this->view_matrix;
}

const glm::vec3 Camera::get_position() const
{
	return this->position;
}