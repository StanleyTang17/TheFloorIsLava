#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 world_up)
{
	this->view_matrix = glm::mat4(1.0f);

	this->sensitivity = 5.0f;

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

void Camera::update_mouse_input(const float& dt, const double& offset_x, const double& offset_y)
{
	this->pitch += static_cast<GLfloat>(offset_y) * this->sensitivity * dt;
	this->yaw += static_cast<GLfloat>(offset_x) * this->sensitivity * dt;

	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	else if (this->pitch < -80.0f)
		this->pitch = -80.0f;

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

const glm::vec3 Camera::get_front() const
{
	return this->front;
}

const glm::vec3 Camera::get_right() const
{
	return this->right;
}

const glm::vec3 Camera::get_world_up() const
{
	return this->world_up;
}

void Camera::set_position(glm::vec3 position)
{
	this->position = position;
}