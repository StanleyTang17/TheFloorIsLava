#pragma once

#ifndef MESH_H
#define MESH_H

#include"engine/shader/Shader.h"
#include"engine/texture/Texture.h"
#include"Primitive.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

constexpr auto MAX_BONE_INFLUENCE = 4;

struct AnimatedVertex : Vertex
{
	float bone_ids[MAX_BONE_INFLUENCE];
	float weights[MAX_BONE_INFLUENCE];
};

class Mesh : public Primitive
{
protected:
	std::vector<Texture2D*> textures;
	
	void update_uniform(Shader* shader);

public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures);
	Mesh(std::vector<AnimatedVertex> vertices, std::vector<GLuint> indices, std::vector<Texture2D*> textures);
	~Mesh();
	void rendor(Shader* vertex_shader, Shader* fragment_shader);
};

#endif