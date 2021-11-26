#include "Random.h"

Random::Random()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	this->mt = mt;
}

int Random::rand_int(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(this->mt);
}

float Random::rand_float(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(this->mt);
}

glm::vec3 Random::rand_vec3(glm::vec3 lower_bound, glm::vec3 upper_bound)
{
	return glm::vec3(
		this->rand_float(lower_bound.x, upper_bound.x),
		this->rand_float(lower_bound.y, upper_bound.y),
		this->rand_float(lower_bound.z, upper_bound.z)
	);
}