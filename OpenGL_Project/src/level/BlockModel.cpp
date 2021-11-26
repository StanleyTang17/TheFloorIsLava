#include "BlockModel.h"

BlockInstance::BlockInstance(Model* model, std::string render_queue, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float top_block_landing_time)
	:
	ModelInstance(model, render_queue, position, rotation, scale)
{
	this->top_block_landing_time = top_block_landing_time;
}

Chunk::Chunk(int rows, int cols, int height, int grid_size)
	:
	ROWS(rows),
	COLS(cols),
	HEIGHT(height),
	GRID_SIZE(grid_size)
{
	int size = rows * cols * height;
	this->instances = new BlockInfo[size];

	for (int h = 0; h < height; ++h)
		for (int r = 0; r < rows; ++r)
			for (int c = 0; c < cols; ++c)
			{
				glm::vec3 pos(r * GRID_SIZE - GRID_SIZE / 2, h * GRID_SIZE - GRID_SIZE / 2, c * GRID_SIZE - GRID_SIZE / 2);
				BlockInfo info = { Utility::generate_transform(pos, glm::vec3(0.0f), glm::vec3(1.0f)), -1.0f, -1.0f };
				this->instances[this->get_index(r, c, h)] = info;
			}

	this->instances[this->get_index(1, 1, 1)].enabled = 1.0f;

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(BlockInfo), this->instances, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Chunk::update_VBO(int index, int length)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	//glBufferSubData(GL_ARRAY_BUFFER, index, length * sizeof(BlockInfo), &this->instances[index]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(ROWS * COLS * HEIGHT * sizeof(BlockInfo) / 2), this->instances);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Chunk::init_VAO(GLuint VAO, GLuint index)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	//glBufferData(GL_ARRAY_BUFFER, (std::size_t)ROWS * COLS * HEIGHT * sizeof(BlockInfo), this->instances, GL_DYNAMIC_DRAW);

	std::size_t vec4_size = sizeof(glm::vec4);

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, sizeof(BlockInfo), (void*)0);

	glEnableVertexAttribArray(index + 1);
	glVertexAttribPointer(index + 1, 4, GL_FLOAT, GL_FALSE, sizeof(BlockInfo), (void*)(1 * vec4_size));

	glEnableVertexAttribArray(index + 2);
	glVertexAttribPointer(index + 2, 4, GL_FLOAT, GL_FALSE, sizeof(BlockInfo), (void*)(2 * vec4_size));

	glEnableVertexAttribArray(index + 3);
	glVertexAttribPointer(index + 3, 4, GL_FLOAT, GL_FALSE, sizeof(BlockInfo), (void*)(3 * vec4_size));

	glEnableVertexAttribArray(index + 4);
	glVertexAttribPointer(index + 4, 1, GL_FLOAT, GL_FALSE, sizeof(BlockInfo), (void*)(4 * vec4_size));

	glEnableVertexAttribArray(index + 5);
	glVertexAttribPointer(index + 5, 1, GL_FLOAT, GL_FALSE, sizeof(BlockInfo), (void*)(4 * vec4_size + sizeof(float)));

	for (GLuint i = 0; i < 6; ++i)
		glVertexAttribDivisor(index + i, 1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Chunk::~Chunk()
{
	delete[] this->instances;
}

BlockModel::BlockModel()
	:
	InstancedModel("res/models/container/container.obj")
{
	glGenBuffers(1, &this->time_buffer);
}

void BlockModel::update_instances()
{
	std::vector<glm::mat4> matrices;
	std::vector<float> times;
	for (ModelInstance* instance : this->instances)
	{
		BlockInstance* block = static_cast<BlockInstance*>(instance);
		matrices.push_back(block->get_model_matrix());
		times.push_back(block->get_top_block_landing_time());
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->instance_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, matrices.size() * sizeof(glm::mat4), matrices.data());

	glBindBuffer(GL_ARRAY_BUFFER, this->time_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, times.size() * sizeof(float), times.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BlockModel::init_instances()
{
	std::vector<glm::mat4> matrices;
	std::vector<float> times;
	for (ModelInstance* instance : this->instances)
	{
		BlockInstance* block = static_cast<BlockInstance*>(instance);
		matrices.push_back(block->get_model_matrix());
		times.push_back(block->get_top_block_landing_time());
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->instance_buffer);
	glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->time_buffer);
	glBufferData(GL_ARRAY_BUFFER, times.size() * sizeof(float), times.data(), GL_DYNAMIC_DRAW);

	for (Mesh* new_mesh : this->meshes)
	{
		glBindVertexArray(new_mesh->get_VAO());
		//new_mesh->init_instanced_mat4(5, this->instance_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->instance_buffer);

		std::size_t vec4_size = sizeof(glm::vec4);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)0);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(1 * vec4_size));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(2 * vec4_size));
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(3 * vec4_size));

		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);

		glBindBuffer(GL_ARRAY_BUFFER, this->time_buffer);

		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
		glVertexAttribDivisor(9, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}