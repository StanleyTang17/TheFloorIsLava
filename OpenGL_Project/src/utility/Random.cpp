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