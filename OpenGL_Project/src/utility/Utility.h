#pragma once

#ifndef GAME_UTILITY_H
#define GAME_UTILITY_H

#include"libs/GLM.h"
#include<math.h>

namespace Utility
{
	glm::mat4 generate_transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
	std::string float_to_str(float number, unsigned int precision = 1);
}

#endif