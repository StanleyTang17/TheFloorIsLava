#include "Ray.h"

Ray::Ray(glm::vec3 p1, glm::vec3 p2, glm::vec3 color)
	:
	Primitive(0, 0, 0, 0, 0)
{
	this->p1 = p1;
	this->p2 = p2;
	this->color = color;

	glm::vec3 input[2] = { this->p1, this->p2 };

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(input), input, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->num_vertices = 2;
}

void Ray::set_position(glm::vec3 p1, glm::vec3 p2)
{
	this->p1 = p1;
	this->p2 = p2;

	glm::vec3 input[2] = { this->p1, this->p2 };

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(input), input);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Ray::render(Shader* fragment_shader)
{
	fragment_shader->set_vec_3f(this->color, "color");
	
	this->draw_vertices(GL_LINES);
}