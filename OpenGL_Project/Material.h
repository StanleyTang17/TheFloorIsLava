#pragma once

// LOCAL
#include"Shader.h"

class Material
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLuint diffuse_texture;
	GLuint specular_texture;
public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLuint diffuse_texture, GLuint specular_texture);
	~Material();
	void send_to_shader(Shader* program);
};