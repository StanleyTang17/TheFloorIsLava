#pragma once

#ifndef MESH_H
#define MESH_H

#include"engine/shader/Shader.h"
#include"engine/texture/Texture.h"
#include"libs/VERTEX.h"

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture2D*> textures;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	
	void update_uniform(Shader* shader);

public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures);
	~Mesh();
	void update(float dt);
	void rendor(Shader* shader, unsigned int num_instances);
	void init_mesh();
	void init_instances(GLuint instance_VBO);
};

#endif