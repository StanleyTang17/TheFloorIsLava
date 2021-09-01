#include"Collision.h"

glm::vec3 global::set_back = glm::vec3(0.0f);

using namespace Collision;

Shape::Shape(glm::vec3* position_ref, glm::vec3* velocity_ref, ShapeType type, Collision::Behavior behavior)
{
	this->type = type;
	this->behavior = behavior;
	this->position = position_ref;
	this->velocity = velocity_ref;
}

Sphere::Sphere(glm::vec3* position_ref, glm::vec3* velocity_ref, Collision::Behavior behavior, float radius)
	:
	Shape(position_ref, velocity_ref, ShapeType::SPHERE, behavior)
{
	this->radius = radius;
}

Box::Box(glm::vec3* position_ref, glm::vec3* velocity_ref, Collision::Behavior behavior, float length, float width, float height)
	:
	Shape(position_ref, velocity_ref, ShapeType::BOX, behavior)
{
	this->length = length;
	this->width = width;
	this->height = height;
}

bool Box::is_in_box(glm::vec3 point)
{
	glm::vec3 pos = this->get_position();
	return
		(point.x <= pos.x + this->width / 2 && point.x >= pos.x - this->width / 2) &&
		(point.y <= pos.y + this->height / 2 && point.y >= pos.y - this->height / 2) &&
		(point.z <= pos.z + this->length / 2 && point.z >= pos.z - this->length / 2);
}

glm::vec3* Box::get_vertices()
{
	glm::vec3 pos = this->get_position();
	float half_length = this->get_length() / 2;
	float half_width = this->get_width() / 2;
	float half_height = this->get_height() / 2;
	
	glm::vec3* vertices = new glm::vec3[8];
	vertices[0] = glm::vec3(pos.x + half_width, pos.y + half_height, pos.z + half_length);
	vertices[1] = glm::vec3(pos.x - half_width, pos.y + half_height, pos.z + half_length);
	vertices[2] = glm::vec3(pos.x + half_width, pos.y - half_height, pos.z + half_length);
	vertices[3] = glm::vec3(pos.x + half_width, pos.y + half_height, pos.z - half_length);
	vertices[4] = glm::vec3(pos.x - half_width, pos.y - half_height, pos.z + half_length);
	vertices[5] = glm::vec3(pos.x + half_width, pos.y - half_height, pos.z - half_length);
	vertices[6] = glm::vec3(pos.x - half_width, pos.y + half_height, pos.z - half_length);
	vertices[7] = glm::vec3(pos.x - half_width, pos.y - half_height, pos.z - half_length);
	
	return vertices;
}

Math::Plane* Box::get_planes()
{
	glm::vec3 pos = this->get_position();
	Math::Plane* planes = new Math::Plane[6];

	// TOP
	planes[0].P0 = pos + glm::vec3(0.0f, this->height / 2, 0.0f);
	planes[0].N = glm::vec3(0.0f, 1.0f, 0.0f);

	// BOTTOM
	planes[1].P0 = pos + glm::vec3(0.0f, -this->height / 2, 0.0f);
	planes[1].N = glm::vec3(0.0f, -1.0f, 0.0f);

	// FRONT
	planes[2].P0 = pos + glm::vec3(0.0f, 0.0f, this->width / 2);
	planes[2].N = glm::vec3(0.0f, 0.0f, 1.0f);

	// BACK
	planes[3].P0 = pos + glm::vec3(0.0f, 0.0f, -this->width / 2);
	planes[3].N = glm::vec3(0.0f, 0.0f, -1.0f);

	// RIGHT
	planes[4].P0 = pos + glm::vec3(this->length / 2, 0.0f, 0.0f);
	planes[4].N = glm::vec3(1.0f, 0.0f, 0.0f);

	// LEFT
	planes[5].P0 = pos + glm::vec3(-this->length / 2, 0.0f, 0.0f);
	planes[5].N = glm::vec3(-1.0f, 0.0f, 0.0f);

	return planes;
}

bool Collision::check(Shape* shape1, Shape* shape2, const float dt)
{
	ShapeType type1 = shape1->get_type();
	ShapeType type2 = shape2->get_type();
	if (type1 == ShapeType::SPHERE && type2 == ShapeType::SPHERE)
		return check(static_cast<Sphere*>(shape1), static_cast<Sphere*>(shape2), dt);
	else if (type1 == ShapeType::BOX && type2 == ShapeType::BOX)
		return check(static_cast<Box*>(shape1), static_cast<Box*>(shape2), dt);
	else if (type1 == ShapeType::SPHERE && type2 == ShapeType::BOX)
		return check(static_cast<Sphere*>(shape1), static_cast<Box*>(shape2), dt);
	else if (type1 == ShapeType::BOX && type2 == ShapeType::SPHERE)
		return check(static_cast<Sphere*>(shape2), static_cast<Box*>(shape1), dt);
	else
		return false;
}

bool Collision::check(Sphere* sphere1, Sphere* sphere2, const float dt)
{
	glm::vec3 center1 = sphere1->get_position();
	glm::vec3 center2 = sphere2->get_position();
	float distance = glm::distance(center1, center2);

	return distance <= sphere1->get_radius() + sphere2->get_radius();
}

bool Collision::check(Box* box1, Box* box2, const float dt)
{
	glm::vec3 center1 = box1->get_position() + box1->get_velocity() * dt;
	glm::vec3 center2 = box2->get_position() + box2->get_velocity() * dt;
	glm::vec3 diff = center2 - center1;

	glm::vec3 abs_diff = glm::vec3(std::abs(diff.x), std::abs(diff.y), std::abs(diff.z));

	bool collide = 
	   (std::abs(diff.x) <= (box1->get_width() + box2->get_width()) / 2 &&
		std::abs(diff.z) <= (box1->get_length() + box2->get_length()) / 2 &&
		std::abs(diff.y) <= (box1->get_height() + box2->get_height()) / 2);

	Behavior behavior1 = box1->get_behavior();
	Behavior behavior2 = box2->get_behavior();

	if (!collide || behavior1 == Behavior::NONE || behavior2 == Behavior::NONE ||
		behavior1 == Behavior::STATIC && behavior2 == Behavior::STATIC)
		return collide;

	// PHYSICAL COLLISION

	Box* kinetic_box = box1;
	Box* static_box = box2;
	if (behavior1 == Behavior::STATIC)
		std::swap(kinetic_box, static_box);

	// expand static box
	glm::vec3 original = glm::vec3(static_box->get_length(), static_box->get_height(), static_box->get_width());
	static_box->set_length(static_box->get_length() + kinetic_box->get_length());
	static_box->set_height(static_box->get_height() + kinetic_box->get_height());
	static_box->set_width(static_box->get_width() + kinetic_box->get_width());

	glm::vec3 kinetic_pos = kinetic_box->get_position();
	glm::vec3 kinetic_vel = kinetic_box->get_velocity();
	Math::Line ray = { kinetic_pos, kinetic_vel };
	Math::Plane* planes = static_box->get_planes();
	bool in_bound = static_box->is_in_box(kinetic_pos);

	glm::vec3 final_pos = kinetic_pos + kinetic_vel;
	glm::vec3 collide_norm = glm::vec3(0.0f);
	for (std::size_t i = 0; i < 6; ++i)
	{
		glm::vec3 POI = glm::vec3(0.0f);
		float t = -1;
		int result = Math::find_POI(ray, planes[i], POI, t);
		bool valid_t = (t >= 0 && !in_bound) || (t <= 0 && in_bound);

		if (result == 1) // if the line lies on one of the planes
		{
			static_box->set_length(original.x);
			static_box->set_height(original.y);
			static_box->set_width(original.z);

			return false;
		}
		else if (result == 0 && valid_t && static_box->is_in_box(POI))
		{
			if (glm::distance(kinetic_pos, POI) < glm::distance(kinetic_pos, final_pos))
			{
				final_pos = POI;
				collide_norm = planes[i].N;
			}
		}
	}

	kinetic_box->set_position(final_pos);
	kinetic_box->set_velocity(kinetic_vel + collide_norm * glm::abs(kinetic_vel));

	delete[] planes;
	planes = nullptr;
	
	static_box->set_length(original.x);
	static_box->set_height(original.y);
	static_box->set_width(original.z);

	return true;
}

bool Collision::check(Sphere* sphere, Box* box, const float dt)
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

	return glm::distance(closest_point, center1) <= sphere->get_radius();

}