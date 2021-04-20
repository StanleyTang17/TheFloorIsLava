#pragma once

#include"Shader.h"
#include"Primitives.h"
#include"Texture.h"

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture*> textures;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 model_matrix;

	
	void update_uniform(Shader* shader);
	void update_model_matrix();

public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture*> textures);
	~Mesh();
	void rendor(Shader* shader);
	void init_mesh();
	void set_position(glm::vec3 position);
	void set_rotation(glm::vec3 rotation);
	void set_scale(glm::vec3 scale);
	void move(glm::vec3 translate);
	void rotate(glm::vec3 rotate);
	void _scale(glm::vec3 scale);
};