#pragma once

#ifndef GAME_UTILITY_H
#define GAME_UTILITY_H

#include"libs/GLM.h"
#include<math.h>
#include<vector>

namespace Utility
{
	glm::mat4 generate_transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
	std::string float_to_str(float number, unsigned int precision = 1);

	float polynomial_interpolate(std::pair<float, float> p1, std::pair<float, float> p2, float cur);
	glm::vec3 polynomial_interpolate(std::pair<float, glm::vec3> p1, std::pair<float, glm::vec3> p2, float cur);
	float linear_interpolate(std::pair<float, float> p1, std::pair<float, float> p2, float cur);
	glm::vec3 linear_interpolate(std::pair<float, glm::vec3> p1, std::pair<float, glm::vec3> p2, float cur);

	class t_float
	{
	private:
		std::vector<std::pair<float, float>>  time_points;
	public:
		inline void add_point(float time, float value) { this->time_points.push_back(std::pair<float, float>(time, value)); }
		inline std::pair<float, float> get_point(std::size_t index) { return this->time_points[index]; }
		inline void set_point(std::size_t index, float time, float value) { this->time_points[index] = std::pair<float, float>(time, value); }
		inline void set_value(std::size_t index, float value) { this->time_points[index].second = value; }
		inline std::pair<float, float> get_start() const { return this->time_points.empty() ? std::make_pair(0.0f, 0.0f) : this->time_points[0]; }
		inline std::pair<float, float> get_end() const { return this->time_points.empty() ? std::make_pair(0.0f, 0.0f) : this->time_points[this->time_points.size() - 1]; }
		float linear_interpolate(float cur_time);
		float polynomial_interpolate(float cur_itme);
	};

	class t_vec3
	{
	private:
		std::vector<std::pair<float, glm::vec3>>  time_points;
	public:
		inline void add_point(float time, glm::vec3 value) { this->time_points.push_back(std::pair<float, glm::vec3>(time, value)); }
		inline std::pair<float, glm::vec3> get_point(std::size_t index) { return this->time_points[index]; }
		inline void set_point(std::size_t index, float time, glm::vec3 value) { this->time_points[index] = std::pair<float, glm::vec3>(time, value); }
		inline void set_value(std::size_t index, glm::vec3 value) { this->time_points[index].second = value; }
		inline std::pair<float, glm::vec3> get_start() const { return this->time_points.empty() ? std::make_pair(0.0f, glm::vec3(0.0f)) : this->time_points[0]; }
		inline std::pair<float, glm::vec3> get_end() const { return this->time_points.empty() ? std::make_pair(0.0f, glm::vec3(0.0f)) : this->time_points[this->time_points.size() - 1]; }
		glm::vec3 linear_interpolate(float cur_time);
		glm::vec3 polynomial_interpolate(float cur_itme);
	};
}

#endif