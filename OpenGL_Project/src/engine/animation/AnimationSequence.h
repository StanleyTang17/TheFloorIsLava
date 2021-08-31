#pragma once

#ifndef ANIMATION_SEQUENCE_H
#define ANIMATION_SEQUENCE_H

#include"AnimationBone.h"
#include"AnimationModel.h"

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	std::vector<std::size_t> children_indices;
};

class Animation::Sequence
{
private:
	float duration;
	int ticks_per_second;
	std::vector<Animation::Bone*> bones;
	std::vector<AssimpNodeData> nodes;
	std::map<std::string, BoneInfo> bone_info_map;

	void read_missing_bones(const aiAnimation* animation, Animation::Model* model);
	void read_heirarchy_data(std::size_t index, const aiNode* src);
		
public:
	Sequence(std::string animation_path, Animation::Model* model, std::size_t animation_index = 0);
	~Sequence();

	Animation::Bone* find_bone(std::string name);
	float get_duration() const { return this->duration; }
	int get_ticks_per_second() const { return this->ticks_per_second; }
	AssimpNodeData get_node(std::size_t index) { return this->nodes[index]; }
	std::map<std::string, BoneInfo> get_bone_info_map() { return this->bone_info_map; }
};

#endif