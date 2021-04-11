#pragma once

#include"Mesh.h"
#include"Material.h"
#include"Texture.h"
#include<iostream>

class Model
{
private:
	Material* material;
	Texture* override_diffuse;
	Texture* override_specular;
	std::vector<Mesh*> meshes;
	glm::vec3 position;

	void update_uniforms();

public:
	Model(glm::vec3 position, Material* material, Texture* override_diffuse, Texture* override_specular, std::vector<Mesh*> meshes);
	~Model();
	void update();
	void render(Shader* shader);
};

