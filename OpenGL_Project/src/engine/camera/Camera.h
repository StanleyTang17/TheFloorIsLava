#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include"libs/OPENGL.h"
#include"libs/GLM.h"
#include"engine/input/MouseMoveInput.h"

#include<math.h>

enum direction { FORWARD = 0, LEFT = 1, BACK = 2, RIGHT = 3 };

class Camera : public MouseMoveInput
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

	GLfloat sensitivity;

public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 world_up);
	~Camera();

	void handle_mouse_move_input(const float dt, const double offset_x, const double offset_y) override;
	void update_camera_vectors();

	const glm::mat4 get_view_matrix();
	const glm::vec3 get_position() const;
	const glm::vec3 get_front() const;
	const glm::vec3 get_right() const;
	const glm::vec3 get_world_up() const;
	const glm::vec3 get_camera_up() const;

	void set_position(glm::vec3 position);
	void set_front(glm::vec3 front);
};

#endif