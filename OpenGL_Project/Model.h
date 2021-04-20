#pragma once

#include"Mesh.h"
#include"NATIVE.h"
#include"ASSIMP.h"

class Model
{
private:
	std::vector<Mesh*> meshes;
	std::vector<Texture*> textures_loaded;
	std::string directory;
	glm::vec3 position;

	void update_uniforms();

	void load_node(aiNode* node, const aiScene* scene);
	void load_mesh(aiMesh* mesh, const aiScene* scene);

public:
	Model(const char* path);
	~Model();
	void load_model(std::string path);
	void update();
	void render(Shader* shader);
};

