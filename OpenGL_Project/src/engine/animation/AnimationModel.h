#pragma once

#ifndef ANIMATION_MODEL_H
#define ANIMATION_MODEL_H

#include"AnimationMesh.h"
#include"utility/AssimpToGLM.h"

struct Animation::BoneInfo
{
	int id;
	glm::mat4 offset;
};

class Animation::Model
{
private:
	std::vector<Mesh*> meshes;
	std::vector<Texture2D*> textures_loaded;
	std::map<std::string, BoneInfo> bone_info_map;
	std::string directory;
	int bone_count;

	void load_node(aiNode* node, const aiScene* scene);
	void load_mesh(aiMesh* mesh, const aiScene* scene);
	void set_vertex_bone_default(Vertex& vertex);
	void append_vertex_bone(Vertex& vertex, int bone_id, float weight);
	void extract_bone_info(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

public:
	Model(std::string path);
	~Model();
	void load_model(std::string path);
	void update(const float dt);
	void render(Shader* shader);

	std::map<std::string, BoneInfo> get_bone_info_map() { return this->bone_info_map; }
	int get_bone_count() const { return this->bone_count; }

	std::vector<Mesh*> get_meshes() { return this->meshes; }
};

#endif