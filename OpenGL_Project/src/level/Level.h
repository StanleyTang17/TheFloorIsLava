#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#include<iostream>
#include<list>
#include<math.h>
#include"gameobject/player/Player.h"
#include"engine/render_queue/RenderQueue.h"
#include"engine/input/MouseMoveInput.h"
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
	const int ROWS;
	const int COLS;
	const int MAX_HEIGHT;
	unsigned int* height_map;

	Player player;
	Camera camera;

	std::list<GameObject*> gameobjects;
	std::list<QueuedBlock> queued_blocks;
	std::string render_queue;
	
	inline int get_index(int row, int col) const { return row * COLS + col; }
	unsigned int get_height(int row, int col) { return this->height_map[this->get_index(row, col)]; }
	glm::ivec3 get_grid_pos(glm::vec3 position);
	void check_collision(GameObject *object, const float dt);
	void check_x_collision(GameObject *object, const float dt);
	void check_y_collision(GameObject *object, const float dt);
	void check_z_collision(GameObject *object, const float dt);

public:
	Level(const int rows, const int cols, const int height, std::string queue);
	~Level();

	static const int GRID_SIZE = 2;

	void update(const float dt);
	void queue_block(int row, int col, float time_til_landing);
	void handle_key_input(GLFWwindow* window, int key, int action) override;
	void handle_mouse_move_input(const float dt, const double offset_x, const double offset_y) override;
	void add_gameobject(GameObject* object);

	glm::vec3 get_player_pos() const { return this->player.get_position(); }

	inline Camera get_camera() const { return this->camera; }
	
};

#endif