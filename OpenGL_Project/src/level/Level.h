#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#include<iostream>
#include<list>
#include<math.h>
#include<fstream>
#include"gameobject/player/Player.h"
#include"engine/render_queue/ModelRenderQueue.h"
#include"engine/render_queue/TextRenderQueue.h"
#include"engine/model/InstancedModel.h"
#include"engine/input/MouseMoveInput.h"
#include"engine/particles/ParticleEffect.h"
#include"engine/font/Font.h"
#include"engine/lights/Lights.h"
#include"engine/mesh/Image.h"
#include"utility/Timer.h"
#include"models/LavaModel.h"
#include"models/WarehouseModel.h"

struct QueuedBlock
{
	int row;
	int col;
	float landing_time;
	ModelInstance* block_instance;
	ModelInstance* warning_instance;
};

enum class LevelState
{
	START, RUNNING, GAME_OVER
};

class Level : public KeyInput, public MouseMoveInput
{
private:
	const float QUEUE_TIME_PER_BLOCK = 0.25f;
	const int NUM_QUEUED_BLOCKS = 20;
	const float DROP_ACCELERATION = 80.0f;
	const int DROP_HEIGHT_OFFSET = 10;
	const float DROP_DURATION;

	LevelState state;

	int* height_map;
	bool* queue_map;

	Image* text_bg;
	ModelInstance* lava_instance;
	ModelInstance* warehouse_instance;
	bool has_lava;
	float lava_speed = 0.1f;

	float height_threshold;
	float height_increment;

	Timer timer;
	float time_survived;
	float highscore;
	std::string save_file;

	Player player;
	Camera camera;

	Random random;
	ParticleEffect* particles;

	TextInfo title_text;
	TextInfo countdown_text;
	TextInfo time_survived_text;
	TextInfo highscore_text;
	TextInfo help_text;

	// Camera Animation
	Utility::t_vec3 camera_anim_pos;
	Utility::t_vec3 camera_anim_axes;
	Utility::t_float text_alpha;
	Utility::t_float text_bg_alpha;

	std::list<GameObject*> gameobjects;
	std::list<QueuedBlock> queued_blocks;
	std::unordered_map <std::string, bool> instance_updated;

	const int num_lights = 2;
	int last_lit_wall_light_index;
	PointLight** lights;
	
	inline int get_index(int row, int col) const { return row * COLS + col; }
	inline int get_height(int row, int col) { return this->height_map[this->get_index(row, col)]; }
	inline bool is_tile_queued(int row, int col) { return this->queue_map[this->get_index(row, col)]; }
	inline void set_tile_queued(int row, int col, bool queued) { this->queue_map[this->get_index(row, col)] = queued; }
	glm::ivec3 get_grid_pos(glm::vec3 position);
	void calc_light_placement(int iteration, glm::vec3& position, glm::vec3& rotation);

	void update_gameobjects(const float dt);
	void queue_blocks(const float wait_time = 0.0f);
	void drop_blocks();
	void update_lava(const float dt);
	void update_walls();
	void update_lights();
	void play_post_game_animation();

	float read_high_score();
	void write_high_score(float highscore);

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

	Level(const int rows, const int cols, const int height);
	~Level();

	static const int GRID_SIZE = 2;

	void update(const float dt);
	void render_particles(Shader* fragment_shader);
	void render_lava(Shader* vertex_shader, Shader* fragment_shader);
	void render_text_bg(Shader* vertex_shader, Shader* fragment_shader);
	void queue_block(int row, int col, float time_til_landing);
	void handle_key_input(GLFWwindow* window, int key, int action) override;
	void handle_mouse_move_input(const float dt, const double offset_x, const double offset_y) override;
	void add_gameobject(GameObject* object);

	glm::vec3 get_player_pos() const { return this->player.get_position(); }

	inline Camera get_camera() const { return this->camera; }
	inline float get_time_survived() const { return this->time_survived; }
	inline bool is_game_over() const { return this->state == LevelState::GAME_OVER; }
	inline PointLight** get_lights() const { return this->lights; }
};

#endif