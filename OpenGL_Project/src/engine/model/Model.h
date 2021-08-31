#pragma once

#ifndef MODEL_H
#define MODEL_H

#include"engine/mesh/Mesh.h"
#include"engine/model/ModelInstance.h"
#include"libs/ASSIMP.h"


enum class ModelClass
{
	Undefined, Ball, Box, GrassPlane, GlassPane, BrickWall, Zombie
};

class Model
{
private:
	std::vector<Mesh*> meshes;
	std::vector<Texture2D*> textures_loaded;
	std::vector<ModelInstance*> instances;
	std::string directory;

	GLuint instance_VBO;

	void load_node(aiNode* node, const aiScene* scene);
	void load_mesh(aiMesh* mesh, const aiScene* scene);

public:
	Model(const char* path);
	~Model();
	void load_model(std::string path);
	void update(float dt);
	void render(Shader* shader);
	void init_instances();

	void add_instance(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void add_instance(ModelInstance* instance);
	void remove_instance(int index = -1);
	void remove_instance(ModelInstance* instance);
	ModelInstance* get_instance(std::size_t index);
	int get_num_instances() const { return this->instances.size(); }
};

#endif