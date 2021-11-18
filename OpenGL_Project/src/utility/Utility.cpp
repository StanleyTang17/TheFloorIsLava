#include "Utility.h"

glm::mat4 Utility::generate_transform(const glm::vec3 translation, const glm::vec3 rotation, const glm::vec3 scale)
{
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, translation);
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	matrix = glm::scale(matrix, scale);
	return matrix;
}

std::string Utility::float_to_str(float number, unsigned int precision)
{
	float power = std::pow(number, precision);
	number = (int)(number * power) / power;
	std::string str = std::to_string(number);
	std::size_t period_index = str.find_first_of('.');
	return str.substr(0, period_index + 1 + precision);
}