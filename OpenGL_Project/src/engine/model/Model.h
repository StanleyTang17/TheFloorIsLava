#pragma once

#ifndef MODEL_H
#define MODEL_H

#include"engine/mesh/Mesh.h"
#include"libs/ASSIMP.h"
#include"utility/AssimpToGLM.h"


enum class ModelClass
{
	Undefined, Ball, Box, GrassPlane, GlassPane, BrickWall, Zombie
};

class Model
{
protected:
	static std::map<std::string, Model*> LOADED_SET;

	std::vector<Mesh*> meshes;
	std::vector<Texture2D*> textures_loaded;
	std::string directory;
	std::string name;
	bool animated;

	void load_node(aiNode* node, const aiScene* scene);
	void load_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Vertex> load_vertices(aiMesh* mesh);
	std::vector<GLuint> load_indices(aiMesh* mesh);
	std::vector<Texture2D*> load_textures(aiMesh* mesh, const aiScene* scene);
	static std::map<std::string, Model*> create_loaded_set();

public:
	Model(std::string path);
	~Model();
	void render(Shader* shader);
	bool is_animated() const { return this->animated; }
	std::string get_name() const { return this->name; }
	
	static Model* get_loaded_model(std::string model_name);
	static bool remove_loaded_model(std::string model_name);
	static void load_model(std::string path);
};

#endif