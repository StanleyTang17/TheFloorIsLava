#pragma once

#ifndef RAY_H
#define RAY_H

#include"engine/shader/Shader.h"
#include"engine/mesh/Primitive.h"

class Ray : public Primitive
{
private:
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 color;

public:
	Ray(glm::vec3 p1, glm::vec3 p2, glm::vec3 color);
	void set_position(glm::vec3 p1, glm::vec3 p2);
	void render(Shader* fragment_shader);

	inline glm::vec3 get_p1() const { return this->p1; }
	inline glm::vec3 get_p2() const { return this->p2; }
	inline glm::vec3 get_color() const { return this->color; }
};

#endif