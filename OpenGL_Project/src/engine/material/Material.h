#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include"engine/shader/Shader.h"

class Material
{
private:
	GLuint diffuse_texture;
	GLuint specular_texture;
	float shininess;
public:
	Material(GLuint diffuse_texture, GLuint specular_texture, float shininess);
	~Material();
	void send_to_shader(Shader* program);
};

#endif