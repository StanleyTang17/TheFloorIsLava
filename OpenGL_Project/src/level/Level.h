#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#include<iostream>
#include<list>
#include<math.h>
#include"gameobject/player/Player.h"
#include"engine/render_queue/RenderQueue.h"
#include"engine/model/InstancedModel.h"
#include"engine/input/MouseMoveInput.h"
#include"engine/particles/ParticleEffect.h"
#include"utility/Timer.h"
#include"GlobalDebug.h"

struct QueuedBlock
{
	int row;
	int col;
	float landing_time;
};

class Level : public KeyInput, public MouseMoveInput
{
private:
	const float QUEUE_TIME_PER_BLOCK = 0.25f;
	const int NUM_QUEUED_BLOCKS = 20;

	int* height_map;
	bool* queue_map;

	ModelInstance* lava_instance;
	bool has_lava;
	float lava_speed = 0.05f;

	Timer timer;
	bool game_over;
	float time_survived;

	Player player;
	Camera camera;

	Random random;
	ParticleEffect particles;

	std::list<GameObject*> gameobjects;
	std::list<QueuedBlock> queued_blocks;
	std::string render_queue;
	
	inline int get_index(int row, int col) const { return row * COLS + col; }
	inline int get_height(int row, int col) { return this->height_map[this->get_index(row, col)]; }
	inline bool is_tile_queued(int row, int col) { return this->queue_map[this->get_index(row, col)]; }
	inline void set_tile_queued(int row, int col, bool queued) { this->queue_map[this->get_index(row, col)] = queued; }
	glm::ivec3 get_grid_pos(glm::vec3 position);
	void update_gameobjects(const float dt);
	void queue_blocks();
	void drop_blocks();
	void update_lava(const float dt);
	void terminate();
	void restart();
	void check_collision(GameObject *object, const float dt);
	void check_x_collision(GameObject *object, const float dt);
	void check_y_collision(GameObject *object, const float dt);
	void check_z_collision(GameObject *object, const float dt);

public:
	const int ROWS;
	const int COLS;
	const int MAX_HEIGHT;

	Level(const int rows, const int cols, const int height, std::string queue);
	~Level();

	static const int GRID_SIZE = 2;

	void update(const float dt);
	void render_particles();
	void queue_block(int row, int col, float time_til_landing);
	void handle_key_input(GLFWwindow* window, int key, int action) override;
	void handle_mouse_move_input(const float dt, const double offset_x, const double offset_y) override;
	void add_gameobject(GameObject* object);

	glm::vec3 get_player_pos() const { return this->player.get_position(); }

	inline Camera get_camera() const { return this->camera; }
	inline float get_time_survived() const { return this->time_survived; }
	inline bool is_game_over() const { return this->game_over; }
	
};

#endif