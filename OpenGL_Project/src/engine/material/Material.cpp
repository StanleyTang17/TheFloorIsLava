#include"Material.h"

Material::Material(GLuint diffuse_texture, GLuint specular_texture, float shininess)
{
	this->diffuse_texture = diffuse_texture;
	this->specular_texture = specular_texture;
	this->shininess = shininess;
}

Material::~Material() {}

void Material::send_to_shader(Shader* program)
{
	program->set_1i(this->diffuse_texture, "material.diffuse_map");
	program->set_1i(this->specular_texture, "material.specular_map");
	program->set_1f(this->shininess, "material.shininess");
}