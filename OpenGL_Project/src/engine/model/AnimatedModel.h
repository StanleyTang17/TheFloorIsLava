#pragma once

#ifndef ANIMATED_MODEL_H
#define ANIMATED_MODEL_H

#include"Model.h"
#include"AnimatedBone.h"
#include<fstream>
#include<sstream>
#include<map>

struct BoneInfo
{
	int id;
	glm::mat4 offset;
};

struct AnimatedNode
{
	std::string name;
	glm::mat4 transformation;
	std::vector<std::size_t> children_indices;
};

struct Sequence
{
	std::string name;
	float start_time;
	float end_time;
};

class AnimatedModel : public Model
{
private:
	std::map<std::string, BoneInfo> bone_info_map;
	std::vector<AnimatedNode> animated_nodes;
	std::vector<AnimatedBone> animated_bones;
	std::vector<glm::mat4> bone_matrices;
	std::map<std::string, Sequence> animations;
	int bone_count;
	int FPS;

	void set_vertex_bone_default(AnimatedVertex& vertex);
	void append_vertex_bone(AnimatedVertex& vertex, int bone_id, float weight);
	void extract_bone_info(std::vector<AnimatedVertex>& vertices, aiMesh* mesh, const aiScene* scene);
	void read_animated_node(std::size_t index, const aiNode* src);
	void read_animated_bones(aiAnimation* animation, const aiScene* scene);
	void read_split_animations(std::string split_file);
	void calculate_bone_transform(std::size_t index, glm::mat4 parent_transform, const float animation_time);
	AnimatedBone* find_bone_by_name(std::string name);

protected:
	std::vector<AnimatedVertex> load_vertices(aiMesh* mesh, const aiScene* scene);
	void load_mesh(aiMesh* mesh, const aiScene* scene) override;

public:
	AnimatedModel(std::string model_file, std::string split_animation_file);
	std::vector<glm::mat4> get_bone_matrices(const float animation_time);
	inline std::map<std::string, Sequence> get_animations() { return this->animations; }
	inline int get_FPS() const { return this->FPS; }

	static void load(std::string model_path, std::string split_animation_path);
};

#endif