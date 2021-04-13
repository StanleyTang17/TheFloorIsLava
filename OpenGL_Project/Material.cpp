#include"Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLuint diffuse_texture, GLuint specular_texture)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->diffuse_texture = diffuse_texture;
	this->specular_texture = specular_texture;
}

Material::~Material() {}

void Material::send_to_shader(Shader* program)
{
	program->set_vec_3f(this->ambient, "light.ambient");
	program->set_vec_3f(this->diffuse, "light.diffuse");
	program->set_vec_3f(this->specular, "light.specular");
	program->set_1i(this->diffuse_texture, "material.diffuse_map");
	program->set_1i(this->specular_texture, "material.specular_map");
}