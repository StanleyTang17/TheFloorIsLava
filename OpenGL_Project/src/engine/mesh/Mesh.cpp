#include"Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures)
	:
	Primitive(0, 0, 0, 0, 0)
{
	this->num_vertices = vertices.size();
	this->num_indices = indices.size();
	this->textures = textures;
	
	glCreateVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	if (indices.size() > 0)
	{
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	}
	else
		this->EBO = 0;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}

Mesh::Mesh(std::vector<AnimatedVertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures)
	:
	Primitive(0, 0, 0, 0, 0)
{
	this->num_vertices = vertices.size();
	this->num_indices = indices.size();
	this->textures = textures;

	glCreateVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(AnimatedVertex), vertices.data(), GL_STATIC_DRAW);

	if (indices.size() > 0)
	{
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),indices.data(), GL_STATIC_DRAW);
	}
	else
		this->EBO = 0;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, texcoord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, tangent));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, bitangent));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, bone_ids));
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, weights));
	glEnableVertexAttribArray(6);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	for (std::size_t i = 0; i < this->textures.size(); ++i)
		delete textures[i];
}

void Mesh::update_uniform(Shader* shader)
{
	unsigned int num_diffuse = 1;
	unsigned int num_specular = 1;
	unsigned int num_normal = 1;
	unsigned int num_height = 1;

	for (std::size_t i = 0; i < this->textures.size(); ++i)
	{
		Texture* tex = this->textures[i];
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, tex->get_id());

		std::string name = tex->get_type();
		unsigned int number = 0;

		if (name == "texture_diffuse")
			number = num_diffuse++;
		else if (name == "texture_specular")
			number = num_specular++;
		else if (name == "texture_normal")
			number = num_normal++;
		else if (name == "texture_height")
			number = num_height++;

		std::string combined = name + std::to_string(number);
		shader->set_1i(i, combined.c_str());
	}
}

void Mesh::rendor(Shader* vertex_shader, Shader* fragment_shader)
{
	this->update_uniform(fragment_shader);
	this->draw_vertices();

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}