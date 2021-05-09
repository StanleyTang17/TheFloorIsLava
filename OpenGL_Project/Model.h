#pragma once

#include"Mesh.h"
#include"NATIVE.h"
#include"ASSIMP.h"
#include"ModelInstance.h"

class Model
{
private:
	std::vector<Mesh*> meshes;
	std::vector<Texture2D*> textures_loaded;
	std::vector<ModelInstance> instances;
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
	void add_instance(ModelInstance instance);
	void remove_instance(int index = -1);
	ModelInstance get_instance(std::size_t index);
	int get_num_instances() const { return this->instances.size(); }
};

