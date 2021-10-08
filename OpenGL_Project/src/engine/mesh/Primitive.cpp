#include "Primitive.h"

Primitive::Primitive(unsigned int num_vertices, GLuint VAO, GLuint VBO, unsigned int num_indices, GLuint EBO)
{
	this->VAO = VAO;
	this->VBO = VBO;
	this->EBO = EBO;
	this->num_vertices = num_vertices;
	this->num_indices = num_indices;
}

Primitive::~Primitive()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	if (this->num_indices > 0)
		glDeleteBuffers(1, &this->EBO);
}

void Primitive::draw_vertices(GLenum mode)
{
	glBindVertexArray(this->VAO);

	if (this->num_indices == 0)
		glDrawArrays(mode, 0, this->num_vertices);
	else
		glDrawElements(mode, this->num_indices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

Quad2D::Quad2D(float x, float y, float w, float h)
	:
	Primitive(0, 0, 0, 0, 0)
{
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;

	float vertices[24] = {
		x,     y + h,   0.0f, 0.0f,
		x,     y,       0.0f, 1.0f,
		x + w, y,       1.0f, 1.0f,

		x,     y + h,   0.0f, 0.0f,
		x + w, y,       1.0f, 1.0f,
		x + w, y + h,   1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->num_vertices = 6;
}