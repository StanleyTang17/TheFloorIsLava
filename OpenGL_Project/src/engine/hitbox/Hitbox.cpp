#include "Hitbox.h"

Hitbox::Hitbox(glm::vec3 center, glm::vec3 size)
{
	this->center = center;
	this->size = size;
	this->update_vertices();
}

void Hitbox::update_vertices()
{
	float dx = this->size.x / 2;
	float dy = this->size.y / 2;
	float dz = this->size.z / 2;

	this->vertices[0] = glm::vec3(center.x - dx, center.y + dy, center.z - dz);
	this->vertices[1] = glm::vec3(center.x - dx, center.y + dy, center.z + dz);
	this->vertices[2] = glm::vec3(center.x + dx, center.y + dy, center.z + dz);
	this->vertices[3] = glm::vec3(center.x + dx, center.y + dy, center.z - dz);
	this->vertices[4] = glm::vec3(center.x - dx, center.y - dy, center.z - dz);
	this->vertices[5] = glm::vec3(center.x - dx, center.y - dy, center.z + dz);
	this->vertices[6] = glm::vec3(center.x + dx, center.y - dy, center.z + dz);
	this->vertices[7] = glm::vec3(center.x + dx, center.y - dy, center.z - dz);
}

void Hitbox::set_center(glm::vec3 center)
{
	this->center = center;
	this->update_vertices();
}

void Hitbox::get_vertices(Face face, glm::vec3 vertices[4])
{
	switch (face)
	{
	case Face::BOTTOM:
		this->get_bottom_vertices(vertices);
		break;
	case Face::LEFT:
		this->get_left_vertices(vertices);
		break;
	case Face::RIGHT:
		this->get_right_vertices(vertices);
		break;
	case Face::FRONT:
		this->get_front_vertices(vertices);
		break;
	case Face::BACK:
		this->get_back_vertices(vertices);
		break;
	default:
		break;
	}
}

void Hitbox::get_bottom_vertices(glm::vec3 arr[4])
{
	arr[0] = this->vertices[4];
	arr[1] = this->vertices[5];
	arr[2] = this->vertices[6];
	arr[3] = this->vertices[7];
}

void Hitbox::get_left_vertices(glm::vec3 arr[4])
{
	arr[0] = this->vertices[0];
	arr[1] = this->vertices[1];
	arr[2] = this->vertices[5];
	arr[3] = this->vertices[4];
}

void Hitbox::get_right_vertices(glm::vec3 arr[4])
{
	arr[0] = this->vertices[3];
	arr[1] = this->vertices[2];
	arr[2] = this->vertices[6];
	arr[3] = this->vertices[7];
}

void Hitbox::get_front_vertices(glm::vec3 arr[4])
{
	arr[0] = this->vertices[1];
	arr[1] = this->vertices[2];
	arr[2] = this->vertices[6];
	arr[3] = this->vertices[5];
}

void Hitbox::get_back_vertices(glm::vec3 arr[4])
{
	arr[0] = this->vertices[0];
	arr[1] = this->vertices[3];
	arr[2] = this->vertices[4];
	arr[3] = this->vertices[7];
}