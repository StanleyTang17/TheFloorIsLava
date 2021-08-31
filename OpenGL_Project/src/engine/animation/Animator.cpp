#include"Animator.h"

Animation::Animator::Animator(Animation::Sequence* animation)
{
	this->play_animation(animation);

	this->final_bone_matrices.reserve(100);

	for (std::size_t i = 0; i < 100; ++i)
		this->final_bone_matrices.push_back(glm::mat4(1.0f));
}

void Animation::Animator::play_animation(Animation::Sequence* animation)
{
	this->current_animation = animation;
	this->current_time = 0.0f;
}

void Animation::Animator::update(const float dt)
{
	if (this->current_animation != nullptr)
	{
		this->current_time += current_animation->get_ticks_per_second() * dt;
		this->current_time = std::fmod(this->current_time, this->current_animation->get_duration());
		this->calculate_bone_transform(0, glm::mat4(1.0f));
	}
}

void Animation::Animator::calculate_bone_transform(std::size_t index, glm::mat4 parent_transform)
{
	AssimpNodeData node = this->current_animation->get_node(index);
	std::string node_name = node.name;
	glm::mat4 node_transform = node.transformation;

	Animation::Bone* bone = this->current_animation->find_bone(node_name);

	if (bone != nullptr)
	{
		bone->update(this->current_time);
		node_transform = bone->get_transform();
	}

	glm::mat4 final_transform = parent_transform * node_transform;

	std::map<std::string, BoneInfo> bone_info_map = this->current_animation->get_bone_info_map();
	if (bone_info_map.find(node_name) != bone_info_map.end())
	{
		BoneInfo bone_info = bone_info_map.at(node_name);
		this->final_bone_matrices[bone_info.id] = final_transform * bone_info.offset;
	}

	for (std::size_t i = 0; i < node.children_indices.size(); ++i)
		this->calculate_bone_transform(node.children_indices[i], final_transform);
}