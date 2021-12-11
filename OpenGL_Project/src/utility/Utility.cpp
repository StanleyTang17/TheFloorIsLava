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

float Utility::polynomial_interpolate(std::pair<float, float> p1, std::pair<float, float> p2, float cur)
{
	// std::pair< time, value >
	float a = (p1.second - p2.second) / std::pow((p2.first - p1.first), 2.0f);
	return a * std::pow((cur - p2.first), 2.0f) + p2.second;
}

glm::vec3 Utility::polynomial_interpolate(std::pair<float, glm::vec3> p1, std::pair<float, glm::vec3> p2, float cur)
{
	return glm::vec3(
		Utility::polynomial_interpolate(std::make_pair(p1.first, p1.second.x), std::make_pair(p2.first, p2.second.x), cur),
		Utility::polynomial_interpolate(std::make_pair(p1.first, p1.second.y), std::make_pair(p2.first, p2.second.y), cur),
		Utility::polynomial_interpolate(std::make_pair(p1.first, p1.second.z), std::make_pair(p2.first, p2.second.z), cur)
	);
}

float Utility::linear_interpolate(std::pair<float, float> p1, std::pair<float, float> p2, float cur)
{
	// std::pair< time, value >
	return (p2.second - p1.first) / (p2.first - p1.first) * cur;
}

glm::vec3 Utility::linear_interpolate(std::pair<float, glm::vec3> p1, std::pair<float, glm::vec3> p2, float cur)
{
	// std::pair< time, value >
	return (p2.second - p1.first) / (p2.first - p1.first) * cur;
}

float Utility::t_float::linear_interpolate(float cur_time)
{
	for (std::size_t i = 0; i < this->time_points.size(); ++i)
	{
		if (i < this->time_points.size() - 1)
		{
			std::pair<float, float> p1 = this->time_points[i];
			std::pair<float, float> p2 = this->time_points[i + 1];

			if (cur_time >= p1.first && cur_time <= p2.first)
				return Utility::linear_interpolate(p1, p2, cur_time);
		}
		else
			return this->time_points[i].second;
	}
}

float Utility::t_float::polynomial_interpolate(float cur_time)
{
	for (std::size_t i = 0; i < this->time_points.size(); ++i)
	{
		if (i < this->time_points.size() - 1)
		{
			std::pair<float, float> p1 = this->time_points[i];
			std::pair<float, float> p2 = this->time_points[i + 1];

			if (cur_time >= p1.first && cur_time <= p2.first)
				return Utility::polynomial_interpolate(p1, p2, cur_time);
		}
		else
			return this->time_points[i].second;
	}
}

glm::vec3 Utility::t_vec3::linear_interpolate(float cur_time)
{
	for (std::size_t i = 0; i < this->time_points.size(); ++i)
	{
		if (i < this->time_points.size() - 1)
		{
			std::pair<float, glm::vec3> p1 = this->time_points[i];
			std::pair<float, glm::vec3> p2 = this->time_points[i + 1];

			if (cur_time >= p1.first && cur_time <= p2.first)
				return Utility::linear_interpolate(p1, p2, cur_time);
		}
		else
			return this->time_points[i].second;
	}
}

glm::vec3 Utility::t_vec3::polynomial_interpolate(float cur_time)
{
	for (std::size_t i = 0; i < this->time_points.size(); ++i)
	{
		if (i < this->time_points.size() - 1)
		{
			std::pair<float, glm::vec3> p1 = this->time_points[i];
			std::pair<float, glm::vec3> p2 = this->time_points[i + 1];

			if (cur_time >= p1.first && cur_time <= p2.first)
				return Utility::polynomial_interpolate(p1, p2, cur_time);
		}
		else
			return this->time_points[i].second;
	}
}