#pragma once

#ifndef PARTICLE_EFFECT_H
#define PARTICLE_EFFECT_H

//#include"engine/shader/Shader.h"
#include"engine/mesh/Primitive.h"
#include"utility/Random.h"
#include<list>

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 scale;
	float time_life_end;
};

class ParticleEffect
{
private:
	std::list<Particle> particles;
	Quad3D quad;
	GLuint instance_VBO;

	glm::mat4 view_matrix;
	glm::vec3 camera_position;

	Random random;

	void init_instances();
	void update_instances();
	std::vector<glm::mat4> generate_model_matrices();

public:
	ParticleEffect();

	void update(const float dt, glm::mat4 view_matrix, glm::vec3 camera_position);
	void generate(glm::vec3 position, glm::vec3 scale, unsigned int count, float duration);
	void render();
	void clear();
};

#endif