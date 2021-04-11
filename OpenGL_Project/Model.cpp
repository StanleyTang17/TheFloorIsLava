#include "Model.h"

Model::Model(glm::vec3 position, Material* material, Texture* override_diffuse, Texture* override_specular, std::vector<Mesh*> meshes)
{
	this->position = position;
	this->material = material;
	this->override_diffuse = override_diffuse;
	this->override_specular = override_specular;

	for (Mesh* mesh : meshes)
		this->meshes.push_back(mesh);
}

Model::~Model()
{
	for (Mesh* mesh : this->meshes)
		delete mesh;
}

void Model::update_uniforms()
{

}

void Model::update()
{

}

void Model::render(Shader* shader)
{
	this->material->send_to_shader(shader);

	shader->use();

	this->override_specular->bind(0);
	this->override_diffuse->bind(1);

	for (Mesh* mesh : this->meshes)
		mesh->rendor(shader);

	this->override_specular->unbind();
	this->override_diffuse->unbind();
}