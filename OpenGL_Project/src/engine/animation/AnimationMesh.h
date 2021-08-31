#pragma once

#ifndef ANIMATION_MESH_H
#define ANIMATION_MESH_H

#include"engine/shader/Shader.h"
#include"engine/texture/Texture.h"
#include"Animation.h"

constexpr auto MAX_BONE_INFLUENCE = 4;

struct Animation::Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	float bone_ids[MAX_BONE_INFLUENCE];
	float weights[MAX_BONE_INFLUENCE];
};

class Animation::Mesh
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
	void update(const float dt);
	void rendor(Shader* shader);
	void init_mesh();

	std::vector<Vertex> get_vertices() { return this->vertices; }
};


#endif