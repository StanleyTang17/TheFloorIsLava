#include"Mesh.h"

Mesh::Mesh(Vertex* vertices, const unsigned int& num_vertices, GLuint* indicies, const unsigned int& num_indices)
{
	this->init_VAO(vertices, num_vertices, indicies, num_indices);

	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);

	this->update_model_matrix();
}

Mesh::Mesh(Primitive* primitive)
{
	this->init_VAO(primitive->get_vertices(), primitive->get_num_vertices(), primitive->get_indices(), primitive->get_num_indices());

	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);

	this->update_model_matrix();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	if(this->num_indicies > 0)
		glDeleteBuffers(1, &this->EBO);
}

void Mesh::init_VAO(Vertex* vertices, const unsigned int& num_vertices, GLuint* indices, const unsigned int& num_indices)
{
	this->num_vertices = num_vertices;
	this->num_indicies = num_indices;

	glCreateVertexArrays(1, &this->VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	if (this->num_indicies > 0)
	{
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->num_indicies * sizeof(GLuint), indices, GL_STATIC_DRAW);
	}
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void Mesh::update_uniform(Shader* shader)
{
	shader->set_mat_4fv(this->model_matrix, "model_matrix", GL_FALSE);
}

void Mesh::update_model_matrix()
{
	this->model_matrix = glm::mat4(1.0f);
	this->model_matrix = glm::translate(this->model_matrix, this->position);
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	this->model_matrix = glm::scale(this->model_matrix, this->scale);
}

void Mesh::set_position(glm::vec3 position)
{
	this->position = position;
}

void Mesh::set_rotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void Mesh::set_scale(glm::vec3 scale)
{
	this->scale = scale;
}

void Mesh::move(glm::vec3 translate)
{
	this->position += translate;
}

void Mesh::rotate(glm::vec3 rotate)
{
	this->rotation += rotate;
}

void Mesh::_scale(glm::vec3 scale)
{
	this->scale *= scale;
}

void Mesh::update()
{

}

void Mesh::rendor(Shader* shader)
{
	this->update_model_matrix();
	this->update_uniform(shader);

	shader->use();

	glBindVertexArray(this->VAO);

	if (this->num_indicies == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, this->num_vertices);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, this->num_indicies, GL_UNSIGNED_INT, 0);
	}
}