#pragma once

#ifndef BLOCK_MODEL_H
#define BLOCK_MODEL_H

#include"engine/model/InstancedModel.h"

struct BlockInfo
{
	glm::mat4 model_matrix;
	float top_block_landing_time;
	float enabled;
};

class BlockInstance : public ModelInstance
{
private:
	float top_block_landing_time;
	bool enabled;
public:
	BlockInstance(Model* model, std::string render_queue, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float top_block_landing_time);
	inline float get_top_block_landing_time() const { return this->top_block_landing_time; }
};

class Chunk
{
private:
	BlockInfo* instances;
	GLuint VBO;

public:
	const int ROWS;
	const int COLS;
	const int HEIGHT;
	const int GRID_SIZE;
	Chunk(int rows, int cols, int height, int grid_size);
	~Chunk();
	inline int get_index(int row, int col, int height) { return height * ROWS * COLS + row * COLS + col; }
	inline BlockInfo get_info(int row, int col, int height) { return this->instances[this->get_index(row, col, height)]; }
	inline void set_info(int row, int col, int height, BlockInfo info) { this->instances[this->get_index(row, col, height)] = info; }
	void update_VBO(int index, int length = 1);
	void init_VAO(GLuint VAO, GLuint index);
};

class BlockModel : public InstancedModel
{
private:
	GLuint time_buffer;
public:
	BlockModel();
	void init_instances() override;
	void update_instances() override;
	inline void add_instance(BlockInstance* block_instance) { this->instances.push_back(block_instance); }
};

#endif