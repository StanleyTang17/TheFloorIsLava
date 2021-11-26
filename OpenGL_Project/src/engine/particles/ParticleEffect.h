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

	glm::mat4 view_matrix;
	glm::vec3 camera_position;

	Random random;

	// configurations
	glm::vec3 lower_vel_bound;
	glm::vec3 upper_vel_bound;
	glm::vec3 lower_scale_bound;
	glm::vec3 upper_scale_bound;
	float deceleration;
	int FPS;

	void init_instances();
	void update_instances();
	void generate_particle_data(std::vector<glm::mat4>& matrices, std::vector<float>& indices);

public:
	ParticleEffect(TextureAtlas2D* texture);
	~ParticleEffect();

	void update(const float dt, glm::mat4 view_matrix, glm::vec3 camera_position);
	void generate(glm::vec3 position, unsigned int count);
	void render(Shader* fragment_shader);
	void clear();

	// configurations
	inline void set_vel_range(glm::vec3 lower_vel, glm::vec3 upper_vel) { this->lower_vel_bound = lower_vel; this->upper_vel_bound = upper_vel; }
	inline void set_scale_range(glm::vec3 lower_scale, glm::vec3 upper_scale) { this->lower_scale_bound = lower_scale; this->upper_scale_bound = upper_scale; }
	inline void set_deceleration(float deceleration) { this->deceleration = deceleration; }
	inline void set_FPS(int FPS) { this->FPS = FPS; }
};

#endif