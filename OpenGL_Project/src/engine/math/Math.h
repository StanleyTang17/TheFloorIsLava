#pragma once

#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include"libs/GLM.h"
#include<iostream>
namespace Math
{
	struct Line // L = L0 + L1 * t
	{
		glm::vec3 L0;
		glm::vec3 L1;
	};

	struct Plane // P = P0 + P1 * u + P2 * v
	{
		glm::vec3 P0;
		glm::vec3 N;
	};

	bool find_POI(Line l, Plane p, glm::vec3 &POI, float &t);
}

#endif