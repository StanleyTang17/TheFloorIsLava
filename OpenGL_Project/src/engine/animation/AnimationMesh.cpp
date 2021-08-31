#include"AnimationMesh.h"

Animation::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->init_mesh();
}

Animation::Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	if (this->indices.size() > 0)
		glDeleteBuffers(1, &this->EBO);
	for (std::size_t i = 0; i < this->textures.size(); ++i)
		delete textures[i];
}

void Animation::Mesh::init_mesh()
{
	glCreateVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

	if (this->indices.size() > 0)
	{
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);
	}

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

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bone_ids));
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, weights));
	glEnableVertexAttribArray(6);

	glBindVertexArray(0);
}

void Animation::Mesh::update_uniform(Shader* shader)
{
	unsigned int num_diffuse = 1;
	unsigned int num_specular = 1;
	unsigned int num_normal = 1;
	unsigned int num_height = 1;

	for (std::size_t i = 0; i < this->textures.size(); ++i)
	{
		Texture* tex = this->textures[i];
		glActiveTexture(GL_TEXTURE0 + tex->get_unit());

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

		shader->set_1i(tex->get_unit(), combined.c_str());
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
	}
}

void Animation::Mesh::update(const float dt)
{

}

void Animation::Mesh::rendor(Shader* shader)
{
	this->update_uniform(shader);

	shader->use();
	glBindVertexArray(this->VAO);

	if (this->indices.size() == 0)
		glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	else
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}