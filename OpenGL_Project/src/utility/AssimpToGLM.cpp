#include "AssimpToGLM.h"

glm::mat4 AssimpToGLM::mat4(aiMatrix4x4 matrix)
{
	float matrix_data[16] = {
		matrix.a1, matrix.b1, matrix.c1, matrix.d1,
		matrix.a2, matrix.b2, matrix.c2, matrix.d2,
		matrix.a3, matrix.b3, matrix.c3, matrix.d3,
		matrix.a4, matrix.b4, matrix.c4, matrix.d4
	};
	return glm::make_mat4(matrix_data);
}

glm::vec2 AssimpToGLM::vec2(aiVector2D vector)
{
	return glm::vec2(vector.x, vector.y);
}

glm::vec3 AssimpToGLM::vec3(aiVector3D vector)
{
	return glm::vec3(vector.x, vector.y, vector.z);
}

glm::quat AssimpToGLM::quat(aiQuaternion quat)
{
	return glm::quat(quat.w, quat.x, quat.y, quat.z);
}