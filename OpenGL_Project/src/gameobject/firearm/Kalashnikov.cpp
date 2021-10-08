#include "Kalashnikov.h"

Kalashnikov::Kalashnikov()
	:
	Firearm(
		new ModelInstance("ak_47", "foreground_animated", glm::vec3(1.0f, -0.8f, -1.0f), glm::vec3(-0.10f, 70.0f, 0.0f), glm::vec3(0.7f)),
		glm::vec3(1.0f, -0.8f, -1.0f),
		FiringMode::AUTOMATIC,
		600,
		1.0f,
		30,
		900
	)
{}