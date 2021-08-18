#pragma once

#include"libs/GLM.h"

namespace Collision
{
	enum class ShapeType;

	class Shape;
	class Sphere;
	class Box;

	bool check(Shape* shape1, Shape* shape2);
	bool check(Sphere* sphere1, Sphere* sphere2);
	bool check(Box* box1, Box* box2);
	bool check(Sphere* sphere, Box* box);
};

enum class Collision::ShapeType
{
	BOX,
	SPHERE
};

class Collision::Shape
{
private:
	Collision::ShapeType type;
	glm::vec3* position;
	glm::vec3* velocity;
public:
	Shape(glm::vec3* position_ref, glm::vec3* velocity_ref, Collision::ShapeType type);

	Collision::ShapeType get_type() const { return this->type; }
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
	Sphere(glm::vec3* position_ref, glm::vec3* velocity_ref, float radius);

	float get_radius() const { return this->radius; }
};

class Collision::Box : public Collision::Shape
{
protected:
	float length;
	float width;
	float height;
public:
	Box(glm::vec3* position_ref, glm::vec3* velocity_ref, float length, float width, float height);

	float get_length() const { return this->length; }
	float get_width() const { return this->width; }
	float get_height() const { return this->height; }
};