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

float Utility::polynomial_interpolate(float start, float end, float total_duration, float cur_time)
{
	// y = a * (t - T)^2 + y1
	float a = (start - end) / std::pow(total_duration, 2.0f);
	return a * std::pow((cur_time - total_duration), 2.0f) + end;
}

glm::vec3 Utility::polynomial_interpolate(glm::vec3 start, glm::vec3 end, float total_duration, float cur_time)
{
	return glm::vec3(
		Utility::polynomial_interpolate(start.x, end.x, total_duration, cur_time),
		Utility::polynomial_interpolate(start.y, end.y, total_duration, cur_time),
		Utility::polynomial_interpolate(start.z, end.z, total_duration, cur_time)
	);
}