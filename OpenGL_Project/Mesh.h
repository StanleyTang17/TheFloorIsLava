#pragma once

#include"Shader.h"
#include"Primitives.h"
#include"Texture.h"

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
	void rendor(Shader* shader);
	void init_mesh();
};