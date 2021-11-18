#include "ParticleEffect.h"

ParticleEffect::ParticleEffect()
	:
	quad(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f),
	view_matrix(1.0f),
	camera_position(0.0f)
{
	glGenBuffers(1, &this->instance_VBO);
}

void ParticleEffect::update(const float dt, glm::mat4 view_matrix, glm::vec3 camera_position)
{
	float current_time = glfwGetTime();
	this->view_matrix = view_matrix;
	this->camera_position = camera_position;
	std::size_t num_particles = this->particles.size();

	std::list<Particle>::iterator particle_it = this->particles.begin();
	while (particle_it != this->particles.end())
	{
		if (particle_it->time_life_end > current_time)
		{
			particle_it->position += particle_it->velocity * dt;
			++particle_it;
		}
		else
			particle_it = this->particles.erase(particle_it);
	}

	if (this->particles.size() == num_particles)
		this->update_instances();
	else
		this->init_instances();
}

std::vector<glm::mat4> ParticleEffect::generate_model_matrices()
{
	std::vector<glm::mat4> matrices;

	glm::mat3 transposed_rotate = glm::mat3(glm::transpose(this->view_matrix));

	for (std::list<Particle>::iterator it = this->particles.begin(); it != this->particles.end(); ++it)
	{
		//glm::mat4 model_matrix = Utility::generate_transform(it->position, glm::vec3(0.0f), it->scale);
		glm::mat4 model_matrix = glm::mat4(1.0f);
		model_matrix = glm::translate(model_matrix, it->position);
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				model_matrix[i][j] = transposed_rotate[i][j];
		model_matrix = glm::scale(model_matrix, it->scale);
		
		matrices.push_back(model_matrix);
	}
	return matrices;
}

void ParticleEffect::update_instances()
{
	std::vector<glm::mat4> matrices = this->generate_model_matrices();

	glBindBuffer(GL_ARRAY_BUFFER, this->instance_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, matrices.size() * sizeof(glm::mat4), matrices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleEffect::init_instances()
{
	std::vector<glm::mat4> matrices = this->generate_model_matrices();

	glBindVertexArray(this->quad.get_VAO());

	glBindBuffer(GL_ARRAY_BUFFER, this->instance_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * matrices.size(), matrices.data(), GL_DYNAMIC_DRAW);

	std::size_t vec4_size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(1 * vec4_size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(2 * vec4_size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(3 * vec4_size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleEffect::generate(glm::vec3 position, glm::vec3 scale, unsigned int count, float duration)
{
	float current_time = glfwGetTime();
	for (unsigned int i = 0; i < count; ++i)
	{
		glm::vec3 velocity(
			this->random.rand_float(-10.0f, 10.0f),
			this->random.rand_float(-0.2f, 0.2f),
			this->random.rand_float(-10.0f, 10.0f)
		);

		Particle particle = {
			position,
			velocity,
			scale,
			current_time + duration
		};
		
		this->particles.push_back(particle);
	}

	this->init_instances();
}

void ParticleEffect::render()
{
	glBindVertexArray(this->quad.get_VAO());
	glDrawArraysInstanced(GL_TRIANGLES, 0, this->quad.get_num_vertices(), this->particles.size());
	glBindVertexArray(0);
}

void ParticleEffect::clear()
{
	this->particles.clear();
}