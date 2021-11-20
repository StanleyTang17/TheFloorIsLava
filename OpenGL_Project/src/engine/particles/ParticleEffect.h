#pragma once

#ifndef PARTICLE_EFFECT_H
#define PARTICLE_EFFECT_H

#include"engine/shader/Shader.h"
#include"engine/texture/Texture.h"
#include"engine/mesh/Primitive.h"
#include"utility/Random.h"
#include<list>

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 scale;
	int atlas_index;
	float time_created;
};

class ParticleEffect
{
private:
	std::list<Particle> particles;
	Quad3D quad;
	GLuint matrix_VBO;
	GLuint index_VBO;
	TextureAtlas2D* texture;
	int FPS = 48;

	glm::mat4 view_matrix;
	glm::vec3 camera_position;

	Random random;

	void init_instances();
	void update_instances();
	void generate_particle_data(std::vector<glm::mat4>& matrices, std::vector<float>& indices);

public:
	ParticleEffect(TextureAtlas2D* texture);
	~ParticleEffect();

	void update(const float dt, glm::mat4 view_matrix, glm::vec3 camera_position);
	void generate(glm::vec3 position, glm::vec3 scale, unsigned int count, float duration);
	void render(Shader* fragment_shader);
	void clear();
};

#endif