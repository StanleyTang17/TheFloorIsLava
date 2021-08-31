#pragma once

#ifndef ASSIMP_TO_GLM_H
#define ASSIMP_TO_GLM_H

#include"libs/GLM.h"
#include"libs/ASSIMP.h"
#include"glm/gtx/quaternion.hpp"

namespace AssimpToGLM
{
	glm::vec2 vec2(aiVector2D vector);
	glm::vec3 vec3(aiVector3D vector);
	glm::quat quat(aiQuaternion quat);
	glm::mat4 mat4(aiMatrix4x4 matrix);
};

#endif

