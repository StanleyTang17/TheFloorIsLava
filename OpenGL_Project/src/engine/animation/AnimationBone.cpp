#include "AnimationBone.h"

Animation::Bone::Bone(std::string name, int id, aiNodeAnim* channel)
{
	this->name = name;
	this->id = id;
	this->local_transform = glm::mat4(0.0f);

	for (std::size_t i = 0; i < channel->mNumPositionKeys; ++i)
	{
		aiVectorKey key_data = channel->mPositionKeys[i];
		KeyPosition position = { AssimpToGLM::vec3(key_data.mValue), key_data.mTime };
		this->positions.push_back(position);
	}

	for (std::size_t i = 0; i < channel->mNumRotationKeys; ++i)
	{
		aiQuatKey key_data = channel->mRotationKeys[i];
		KeyRotation rotation = { AssimpToGLM::quat(key_data.mValue), key_data.mTime };
		this->rotations.push_back(rotation);
	}

	for (std::size_t i = 0; i < channel->mNumScalingKeys; ++i)
	{
		aiVectorKey key_data = channel->mScalingKeys[i];
		KeyScale scale = { AssimpToGLM::vec3(key_data.mValue), key_data.mTime };
		this->scales.push_back(scale);
	}
}

void Animation::Bone::update(const float animation_time)
{
	glm::mat4 position_transform = this->interpolate_position(animation_time);
	glm::mat4 rotation_transform = this->interpolate_rotation(animation_time);
	glm::mat4 scale_transform = this->interpolate_scale(animation_time);
	this->local_transform = position_transform * rotation_transform * scale_transform;
}

std::size_t Animation::Bone::get_position_index(const float animation_time)
{
	for (std::size_t i = 0; i < this->positions.size() - 1; ++i)
		if (animation_time < this->positions[i + 1].time_stamp)
			return i;
	assert(0);
}

std::size_t Animation::Bone::get_rotation_index(const float animation_time)
{
	for (std::size_t i = 0; i < this->rotations.size() - 1; ++i)
		if (animation_time < this->rotations[i + 1].time_stamp)
			return i;
	assert(0);
}

std::size_t Animation::Bone::get_scale_index(const float animation_time)
{
	for (std::size_t i = 0; i < this->scales.size() - 1; ++i)
		if (animation_time < this->scales[i + 1].time_stamp)
			return i;
	assert(0);
}

float Animation::Bone::get_scale_factor(float last_time, float next_time, float current_time)
{
	return (current_time - last_time) / (next_time - last_time);
}

glm::mat4 Animation::Bone::interpolate_position(const float animation_time)
{
	if (this->positions.size() == 1)
		return glm::translate(glm::mat4(1.0f), this->positions[0].position);

	std::size_t index = this->get_position_index(animation_time);
	KeyPosition pos0 = this->positions[index];
	KeyPosition pos1 = this->positions[index + 1];
	
	float scale_factor = this->get_scale_factor(pos0.time_stamp, pos1.time_stamp, animation_time);
	glm::vec3 final_pos = glm::mix(pos0.position, pos1.position, scale_factor);

	return glm::translate(glm::mat4(1.0f), final_pos);
}

glm::mat4 Animation::Bone::interpolate_rotation(const float animation_time)
{
	if (this->rotations.size() == 1)
		glm::toMat4(glm::normalize(this->rotations[0].orientation));

	std::size_t index = this->get_rotation_index(animation_time);
	KeyRotation rot0 = this->rotations[index];
	KeyRotation rot1 = this->rotations[index + 1];

	float scale_factor = this->get_scale_factor(rot0.time_stamp, rot1.time_stamp, animation_time);
	glm::quat final_rot = glm::slerp(rot0.orientation, rot1.orientation, scale_factor);

	return glm::toMat4(glm::normalize(final_rot));
}

glm::mat4 Animation::Bone::interpolate_scale(const float animation_time)
{
	if (this->scales.size() == 1)
		glm::scale(glm::mat4(1.0f), this->scales[0].scale);

	std::size_t index = this->get_scale_index(animation_time);
	KeyScale scale0 = this->scales[index];
	KeyScale scale1 = this->scales[index + 1];

	float scale_factor = this->get_scale_factor(scale0.time_stamp, scale1.time_stamp, animation_time);
	glm::vec3 final_scale = glm::mix(scale0.scale, scale1.scale, scale_factor);

	return glm::scale(glm::mat4(1.0f), final_scale);
}