#pragma once

#include"Mesh.h"
#include"NATIVE.h"
#include"ASSIMP.h"

class Model
{
private:
	std::vector<Mesh*> meshes;
	std::vector<Texture2D*> textures_loaded;
	std::string directory;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 model_matrix;

	void update_model_matrix();
	void update_uniforms(Shader* shader);

	void load_node(aiNode* node, const aiScene* scene);
	void load_mesh(aiMesh* mesh, const aiScene* scene);

public:
	Model(const char* path);
	~Model();
	void load_model(std::string path);
	void update(float dt);
	void render(Shader* shader);
	void set_position(glm::vec3 position);
	void set_rotation(glm::vec3 rotation);
	void set_scale(glm::vec3 scale);
	void move(glm::vec3 translate);
	void rotate(glm::vec3 rotate);
	void _scale(glm::vec3 scale);
	glm::vec3 get_position () const { return this->position; }
};

