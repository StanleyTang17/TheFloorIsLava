#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include"libs/GLM.h"
#include"GlobalDebug.h"
#include"engine/math/Math.h"
#include<math.h>
#include<iostream>

namespace Collision
{
	enum class ShapeType;
	enum class Behavior;

	class Shape;
	class Sphere;
	class Box;

	bool check(Shape* shape1, Shape* shape2, const float dt);
	bool check(Sphere* sphere1, Sphere* sphere2, const float dt);
	bool check(Box* box1, Box* box2, const float dt);
	bool check(Sphere* sphere, Box* box, const float dt);
};

enum class Collision::ShapeType
{
	BOX,
	SPHERE
};

enum class Collision::Behavior
{
	STATIC,
	KINETIC,
	NONE
};

class Collision::Shape
{
private:
	Collision::ShapeType type;
	Collision::Behavior behavior;
	
	glm::vec3* position;
	glm::vec3* velocity;
public:
	Shape(glm::vec3* position_ref, glm::vec3* velocity_ref, Collision::ShapeType type, Collision::Behavior behavior);

	Collision::ShapeType get_type() const { return this->type; }
	Collision::Behavior get_behavior() const { return this->behavior; }
	glm::vec3 get_position() const { return *position; }
	glm::vec3 get_velocity() const { return *velocity; }
	void set_position(glm::vec3 position) { *(this->position) = position; }
	void set_velocity(glm::vec3 velocity) { *(this->velocity) = velocity; }
};

class Collision::Sphere : public Collision::Shape
{
protected:
	float radius;
public:
	Sphere(glm::vec3* position_ref, glm::vec3* velocity_ref, Collision::Behavior behavior, float radius);

	float get_radius() const { return this->radius; }
};

class Collision::Box : public Collision::Shape
{
protected:
	float length;
	float width;
	float height;
public:
	Box(glm::vec3* position_ref, glm::vec3* velocity_ref, Collision::Behavior behavior, float length, float width, float height);

	void set_length(float length) { this->length = length; }
	void set_width(float width) { this->width = width; }
	void set_height(float height) { this->height = height; }
	float get_length() const { return this->length; }
	float get_width() const { return this->width; }
	float get_height() const { return this->height; }
	bool is_in_box(glm::vec3 point);
	glm::vec3* get_vertices();
	Math::Plane* get_planes();
};

#endif