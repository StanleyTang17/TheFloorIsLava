#pragma once

#include"Shader.h"
#include"Primitives.h"

class Mesh
{
private:
	unsigned int num_vertices;
	unsigned int num_indicies;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 model_matrix;

	void init_VAO(Vertex* vertices, const unsigned int& num_vertices, GLuint* indices, const unsigned int& num_indices);
	void update_uniform(Shader* shader);
	void update_model_matrix();

public:
	Mesh(Vertex* vertices, const unsigned int& num_vertices, GLuint* indicies, const unsigned int& num_indices);
	Mesh(Primitive* primitive);
	~Mesh();
	void update();
	void rendor(Shader* shader);
	void set_position(glm::vec3 position);
	void set_rotation(glm::vec3 rotation);
	void set_scale(glm::vec3 scale);
	void move(glm::vec3 translate);
	void rotate(glm::vec3 rotate);
	void _scale(glm::vec3 scale);
};