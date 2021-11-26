#pragma once

#ifndef RANDOM_INT_H
#define RANDOM_INT_H

#include<random>
#include"libs/GLM.h"

class Random
{
private:
	std::mt19937 mt;

public:
	Random();

	int rand_int(int min, int max);
	float rand_float(float min, float max);
	glm::vec3 rand_vec3(glm::vec3 lower_bound, glm::vec3 upper_bound);
};

#endif