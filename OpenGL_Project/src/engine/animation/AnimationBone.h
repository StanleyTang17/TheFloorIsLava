#pragma once

#ifndef ANIMATION_BONE_H
#define ANIMATION_BONE_H

#include"Animation.h"
#include"libs/NATIVE.h"
#include"utility/AssimpToGLM.h"

struct KeyPosition
{
	glm::vec3 position;
	float time_stamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float time_stamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float time_stamp;
};

class Animation::Bone
{
private:
	std::vector<KeyPosition> positions;
	std::vector<KeyRotation> rotations;
	std::vector<KeyScale> scales;

	glm::mat4 local_transform;
	std::string name;
	int id;

	std::size_t get_position_index(const float animation_time);
	std::size_t get_rotation_index(const float animation_time);
	std::size_t get_scale_index(const float animation_time);

	float get_scale_factor(float last_time, float next_time, float current_time);

	glm::mat4 interpolate_position(const float animation_time);
	glm::mat4 interpolate_rotation(const float animation_time);
	glm::mat4 interpolate_scale(const float animation_time);

public:
	Bone(std::string name, int id, aiNodeAnim* channel);

	void update(const float animation_time);
	glm::mat4 get_transform() const { return this->local_transform; }
	std::string get_name() const { return this->name; }
	int get_id() const { return this->id; }
};

#endif