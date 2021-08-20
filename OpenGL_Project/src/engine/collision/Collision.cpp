#include"Collision.h"
#include<math.h>
#include<iostream>

glm::vec3 global::diff = glm::vec3(0.0f);

using namespace Collision;

Shape::Shape(glm::vec3* position_ref, glm::vec3* velocity_ref, ShapeType type)
{
	this->type = type;
	this->position = position_ref;
	this->velocity = velocity_ref;
}

Sphere::Sphere(glm::vec3* position_ref, glm::vec3* velocity_ref, float radius)
	:
	Shape(position_ref, velocity_ref, ShapeType::SPHERE)
{
	this->radius = radius;
}

Box::Box(glm::vec3* position_ref, glm::vec3* velocity_ref, float length, float width, float height)
	:
	Shape(position_ref, velocity_ref, ShapeType::BOX)
{
	this->length = length;
	this->width = width;
	this->height = height;
}

bool Collision::check(Shape* shape1, Shape* shape2)
{
	ShapeType type1 = shape1->get_type();
	ShapeType type2 = shape2->get_type();
	if (type1 == ShapeType::SPHERE && type2 == ShapeType::SPHERE)
		return check(static_cast<Sphere*>(shape1), static_cast<Sphere*>(shape2));
	else if (type1 == ShapeType::BOX && type2 == ShapeType::BOX)
		return check(static_cast<Box*>(shape1), static_cast<Box*>(shape2));
	else if (type1 == ShapeType::SPHERE && type2 == ShapeType::BOX)
		return check(static_cast<Sphere*>(shape1), static_cast<Box*>(shape2));
	else if (type1 == ShapeType::BOX && type2 == ShapeType::SPHERE)
		return check(static_cast<Sphere*>(shape2), static_cast<Box*>(shape1));
	else
		return false;
}

bool Collision::check(Sphere* sphere1, Sphere* sphere2)
{
	glm::vec3 center1 = sphere1->get_position();
	glm::vec3 center2 = sphere2->get_position();
	float distance = glm::distance(center1, center2);

	return distance < sphere1->get_radius() + sphere2->get_radius();
}

bool Collision::check(Box* box1, Box* box2)
{
	glm::vec3 center1 = box1->get_position();
	glm::vec3 center2 = box2->get_position();
	glm::vec3 diff = center2 - center1;

	glm::vec3 abs_diff = glm::vec3(std::abs(diff.x), std::abs(diff.y), std::abs(diff.z));

	return
	   (std::abs(diff.x) < (box1->get_length() + box2->get_length()) / 2 &&
		std::abs(diff.z) < (box1->get_width() + box2->get_width()) / 2 &&
		std::abs(diff.y) < (box1->get_height() + box2->get_height()) / 2);
}

bool Collision::check(Sphere* sphere, Box* box)
{
	glm::vec3 center1 = sphere->get_position();
	glm::vec3 center2 = box->get_position();
	float half_length = box->get_length() / 2;
	float half_width = box->get_width() / 2;
	float half_height = box->get_height() / 2;

	glm::vec3 closest_point = glm::vec3(
		std::max(center2.x - half_length, std::min(center2.x + half_length, center1.x)),
		std::max(center2.y - half_height, std::min(center2.y + half_height, center1.y)),
		std::max(center2.z - half_width, std::min(center2.z + half_width, center1.z))
	);

	global::diff = glm::abs(closest_point - center1);

	return glm::distance(closest_point, center1) < sphere->get_radius();

}