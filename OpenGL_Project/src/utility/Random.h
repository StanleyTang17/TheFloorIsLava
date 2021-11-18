#pragma once

#ifndef RANDOM_INT_H
#define RANDOM_INT_H

#include <random>

class Random
{
private:
	std::mt19937 mt;

public:
	Random();

	int rand_int(int min, int max);
	float rand_float(float min, float max);
};

#endif