#pragma once

#ifndef GAME_UTILITY_H
#define GAME_UTILITY_H

#include"libs/GLM.h"
#include<math.h>

namespace Utility
{
	glm::mat4 generate_transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
	std::string float_to_str(float number, unsigned int precision = 1);
	float polynomial_interpolate(float start, float end, float total_duration, float cur_time);
	glm::vec3 polynomial_interpolate(glm::vec3 start, glm::vec3 end, float total_duration, float cur_time);
}

#endif