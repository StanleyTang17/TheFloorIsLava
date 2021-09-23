#include "Kalashnikov.h"

Kalashnikov::Kalashnikov()
	:
	Firearm(
		new ModelInstance("ak_47", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
		glm::vec3(1.0f),
		FiringMode::AUTOMATIC,
		600,
		1.0f,
		30,
		90
	)
{
	//this->set_offset_from_parent(Utility::generate_transform(
	//	glm::vec3(0.25f, 0.5f, 0.0f),
	//	glm::vec3(0.0f, 30.0f, 0.0f),
	//	glm::vec3(1.0f)
	//));

	this->set_offset_from_parent(Utility::generate_transform(
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f)
	));
}