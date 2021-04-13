#pragma once

#include"OPENGL.h"
#include"GLM.h"

#include<math.h>

enum direction {FORWARD = 0, LEFT = 1, BACK = 2, RIGHT = 3};

class Camera
{
private:
	glm::mat4 view_matrix;

	glm::vec3 world_up;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	GLfloat movement_speed;
	GLfloat sensitivity;

	void update_camera_vectors();
public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 world_up);
	~Camera();

	void update_keyboard_input(const float& dt, const int forward_movement, const int side_movement);
	void update_mouse_input(const float& dt, const double& offset_x, const double& offset_y);

	const glm::mat4 get_view_matrix();
	const glm::vec3 get_position() const;
	const glm::vec3 get_front() const;
};