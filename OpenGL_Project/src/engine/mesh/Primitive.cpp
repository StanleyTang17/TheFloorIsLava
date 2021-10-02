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

void Primitive::draw_vertices()
{
	glBindVertexArray(this->VAO);

	if (this->num_indices == 0)
		glDrawArrays(GL_TRIANGLES, 0, this->num_vertices);
	else
		glDrawElements(GL_TRIANGLES, this->num_indices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

Quad2D::Quad2D(float x, float y, float w, float h)
	:
	Primitive(0, 0, 0, 0, 0)
{
	float vertices[6][4] = {
		{ x,     y + h,   0.0f, 0.0f },
		{ x,     y,       0.0f, 1.0f },
		{ x + w, y,       1.0f, 1.0f },

		{ x,     y + h,   0.0f, 0.0f },
		{ x + w, y,       1.0f, 1.0f },
		{ x + w, y + h,   1.0f, 0.0f }
	};

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(this->VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->num_vertices = 6 * 4;
}