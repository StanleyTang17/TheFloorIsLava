#include "ParticleEffect.h"

ParticleEffect::ParticleEffect(TextureAtlas2D* texture_atlas)
	:
	quad(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f),
	view_matrix(1.0f),
	camera_position(0.0f)
{
	glGenBuffers(1, &this->matrix_VBO);
	glGenBuffers(1, &this->index_VBO);
	
	this->texture = texture_atlas;
	this->texture->set_unit(GL_TEXTURE0);

	// configurations
	this->upper_vel_bound = glm::vec3(1.0f);
	this->lower_vel_bound = glm::vec3(-1.0f);
	this->upper_scale_bound = glm::vec3(1.5f);
	this->lower_scale_bound = glm::vec3(0.5f);
	this->deceleration = 0.0f;
	this->FPS = 24;
}

ParticleEffect::~ParticleEffect()
{
	delete this->texture;
}

void ParticleEffect::update(const float dt, glm::mat4 view_matrix, glm::vec3 camera_position)
{
	this->view_matrix = view_matrix;
	this->camera_position = camera_position;

	std::size_t num_particles = this->particles.size();
	std::size_t num_sprites = this->texture->get_num_sprites();
	float current_time = glfwGetTime();
	float seconds_per_frame = 1.0f / this->FPS;

	std::list<Particle>::iterator particle_it = this->particles.begin();

	while (particle_it != this->particles.end())
	{
		particle_it->atlas_index = (int)((current_time - particle_it->time_created) / seconds_per_frame);

		if (particle_it->atlas_index < num_sprites)
		{
			particle_it->velocity += this->deceleration * dt * glm::normalize(particle_it->velocity);
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

void ParticleEffect::generate_particle_data(std::vector<glm::mat4>& matrices, std::vector<float>& indices)
{
	glm::mat3 transposed_rotate = glm::mat3(glm::transpose(this->view_matrix));

	for (std::list<Particle>::iterator it = this->particles.begin(); it != this->particles.end(); ++it)
	{
		glm::mat4 model_matrix = glm::mat4(1.0f);
		model_matrix = glm::translate(model_matrix, it->position);
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				model_matrix[i][j] = transposed_rotate[i][j];
		model_matrix = glm::scale(model_matrix, it->scale);

		matrices.push_back(model_matrix);
		indices.push_back(it->atlas_index);
	}
}

void ParticleEffect::update_instances()
{
	std::vector<glm::mat4> matrices;
	std::vector<float> indices;
	this->generate_particle_data(matrices, indices);

	glBindBuffer(GL_ARRAY_BUFFER, this->matrix_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, matrices.size() * sizeof(glm::mat4), matrices.data());

	glBindBuffer(GL_ARRAY_BUFFER, this->index_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, indices.size() * sizeof(float), indices.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleEffect::init_instances()
{
	std::vector<glm::mat4> matrices;
	std::vector<float> indices;
	this->generate_particle_data(matrices, indices);

	glBindVertexArray(this->quad.get_VAO());
	
	//this->quad.init_instanced_mat4(3, matrix_VBO, sizeof(glm::mat4) * matrices.size(), matrices.data(), GL_DYNAMIC_DRAW);
	//this->quad.init_instanced_float(7, index_VBO, sizeof(float) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->matrix_VBO);
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

	glBindBuffer(GL_ARRAY_BUFFER, this->index_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glVertexAttribDivisor(7, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleEffect::generate(glm::vec3 position, unsigned int count)
{
	float current_time = glfwGetTime();
	for (unsigned int i = 0; i < count; ++i)
	{
		glm::vec3 velocity = this->random.rand_vec3(this->lower_vel_bound, this->upper_vel_bound);
		glm::vec3 scale = this->random.rand_vec3(this->lower_scale_bound, this->upper_scale_bound);

		Particle particle = {
			position,
			velocity,
			scale,
			0,
			current_time
		};
		
		this->particles.push_back(particle);
	}

	this->init_instances();
}

void ParticleEffect::render(Shader* fragment_shader)
{
	this->texture->bind();
	
	fragment_shader->set_1i(0, "texture_atlas");
	fragment_shader->set_1i(this->texture->get_rows(), "atlas_rows");
	fragment_shader->set_1i(this->texture->get_cols(), "atlas_cols");

	glBindVertexArray(this->quad.get_VAO());
	glDrawArraysInstanced(GL_TRIANGLES, 0, this->quad.get_num_vertices(), this->particles.size());

	glBindVertexArray(0);
	this->texture->unbind();
}

void ParticleEffect::clear()
{
	this->particles.clear();
}