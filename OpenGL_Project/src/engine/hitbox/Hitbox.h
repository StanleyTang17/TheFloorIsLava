#pragma once

#ifndef HITBOX_H
#define HITBOX_H

#include"engine/mesh/Primitive.h"
#include "engine/shader/Shader.h"

enum class Face
{
	TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK
};

class Hitbox
{
private:
	glm::vec3 center;
	glm::vec3 size;
	glm::vec3 vertices[8];
	/*
	
	  1 -- 2
	 /|   /|	y  z
	0 -- 3 |	| /
	| 5 -| 6	|/
	|/   |/		*-----x
	4 -- 7

	*/

	void get_bottom_vertices(glm::vec3 arr[4]);
	void get_left_vertices(glm::vec3 arr[4]);
	void get_right_vertices(glm::vec3 arr[4]);
	void get_front_vertices(glm::vec3 arr[4]);
	void get_back_vertices(glm::vec3 arr[4]);

public:
	Hitbox(glm::vec3 center, glm::vec3 size);

	void update_vertices();
	inline glm::vec3 get_size() const { return this->size; }
	void get_vertices(Face face, glm::vec3 vertices[4]);
	void set_center(glm::vec3 center);
};

#endif