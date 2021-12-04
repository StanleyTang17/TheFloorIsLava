#pragma once

#ifndef MODEL_H
#define MODEL_H

#include"engine/mesh/Mesh.h"
#include"libs/ASSIMP.h"
#include"utility/AssimpToGLM.h"
#include<map>
#include<unordered_map>

class Model
{
protected:
	static std::unordered_map<std::string, Model*> LOADED_SET;

	std::vector<Mesh*> meshes;
	std::vector<Texture2D*> textures_loaded;
	std::string directory;
	std::string name;
	bool animated;

	void load_node(aiNode* node, const aiScene* scene);
	virtual void load_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Vertex> load_vertices(aiMesh* mesh);
	std::vector<GLuint> load_indices(aiMesh* mesh);
	std::vector<Texture2D*> load_textures(aiMesh* mesh, const aiScene* scene);

public:
	Model();
	Model(std::string path);
	~Model();
	void init(std::string path);
	virtual void render(Shader* vertex_shader, Shader* fragment_shader);
	bool is_animated() const { return this->animated; }
	std::string get_name() const { return this->name; }
	std::vector<Mesh*> get_meshes() { return this->meshes; }

	static Model* get(std::string model_name);
	static bool remove(std::string model_name);
	static Model* load(std::string path);
	static void add(Model* model);
};

#endif