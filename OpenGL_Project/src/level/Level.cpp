#include "Level.h"

// TODO implement power ups

Level::Level(const int rows, const int cols, const int height)
	:
	ROWS(rows),
	COLS(cols),
	MAX_HEIGHT(height),
	DROP_DURATION(std::sqrt(2.0 * DROP_HEIGHT_OFFSET * GRID_SIZE / DROP_ACCELERATION)),
	player(glm::vec3(rows * GRID_SIZE / 2, 5.0f, cols * GRID_SIZE / 2)),
	camera(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
	int num_tiles = rows * cols;
	this->height_map = new int[num_tiles];
	this->queue_map = new bool[num_tiles];
	this->game_over = false;
	this->time_survived = 0;

	for (int i = 0; i < num_tiles; ++i)
	{
		this->height_map[i] = 0;
		this->queue_map[i] = false;
	}

	this->gameobjects.push_back(&this->player);

	// INIT GRAPHICS

	WarehouseModel* warehouse_model = new WarehouseModel(glm::vec3(rows * GRID_SIZE / 2, MAX_HEIGHT * GRID_SIZE / 2, cols * GRID_SIZE / 2));
	Model::add(warehouse_model);
	ModelRenderQueue::get("static")->add_instance(new ModelInstance(
		warehouse_model->get_name(),
		"static",
		glm::vec3(rows * GRID_SIZE / 2 - GRID_SIZE, MAX_HEIGHT * GRID_SIZE / 2, cols * GRID_SIZE / 2 - GRID_SIZE),
		glm::vec3(0.0f),
		glm::vec3(1.0f)
	));

	this->lava_model = new LavaModel("res/images/flowmap_2.png");
	Model::add(this->lava_model);
	this->lava_instance = new ModelInstance(
		lava_model->get_name(),
		"static",
		glm::vec3(this->ROWS * GRID_SIZE / 2 - GRID_SIZE, -10.0f, this->COLS * GRID_SIZE / 2 - GRID_SIZE),
		glm::vec3(0.0f),
		glm::vec3(this->ROWS * GRID_SIZE, MAX_HEIGHT * GRID_SIZE, this->COLS * GRID_SIZE)
	);
	this->has_lava = false;

	ModelRenderQueue::get("static")->add_instance(new ModelInstance(
		"concrete_plane",
		"static",
		glm::vec3(this->ROWS * GRID_SIZE / 2 - GRID_SIZE, 0.0f, this->COLS * GRID_SIZE / 2 - GRID_SIZE),
		glm::vec3(0.0f),
		glm::vec3(this->ROWS * GRID_SIZE, MAX_HEIGHT * GRID_SIZE, this->COLS * GRID_SIZE)
	));
	
	// INIT PARTICLES

	float vel = 10.0;
	TextureAtlas2D* particle_texture = new TextureAtlas2D("texture_diffuse", "res/images/particles/smoke.png", 8, 8, 5 * 8);
	this->particles = new ParticleEffect(particle_texture);
	this->particles->set_vel_range(glm::vec3(-vel, -0.5f, -vel), glm::vec3(vel, 0.5f, vel));
	this->particles->set_scale_range(glm::vec3(0.8f), glm::vec3(1.2f));
	this->particles->set_FPS(80);
	this->particles->set_deceleration(-2 * vel);

	// INIT TEXT

	this->title_text = { "Lava rises in", "game_title", 480.0f, 600.0f, 1.0f, true };
	this->time_survived_text = { "", "game_body", 0.0f, 780.0f, 1.0f, true };
	this->countdown_text = { "", "game_title", 550, 550, 1.0f, true };

	this->title_text.x = Font::get(this->title_text.font)->get_center_x(title_text.text, 1.0f, 0.0f, 1270.0f);

	TextRenderQueue::get("game_text")->add_text(&title_text);
	TextRenderQueue::get("game_text")->add_text(&time_survived_text);
	TextRenderQueue::get("game_text")->add_text(&countdown_text);

	this->queue_blocks(1.5f);
	this->timer.reset();

	this->instance_updated["container"] = false;
	this->instance_updated["container_plane"] = false;
}

Level::~Level()
{
	delete[] this->height_map;
	delete[] this->queue_map;
	delete this->lava_instance;
	delete this->particles;
	for (GameObject* object : this->gameobjects)
		delete object;
}

void Level::queue_block(int row, int col, float time_til_landing)
{
	glm::vec3 position = glm::vec3(
		row * GRID_SIZE - GRID_SIZE / 2,
		(float)this->get_height(row, col) * GRID_SIZE + 0.0001f,
		col * GRID_SIZE - GRID_SIZE / 2
	);

	ModelInstance* warning_instance = new ModelInstance("container_plane", "static", position, glm::vec3(0.0f), glm::vec3(1.0f));

	QueuedBlock block{ row, col, glfwGetTime() + time_til_landing, nullptr, warning_instance };
	this->queued_blocks.push_back(block);
	this->set_tile_queued(row, col, true);
	
	InstancedModel::get("container_plane")->add_instance(warning_instance);
	this->instance_updated["container_plane"] = true;
}

void Level::update(const float dt)
{
	this->particles->update(dt, this->camera.get_view_matrix(), this->camera.get_position());

	if (this->game_over)
		return;

	this->instance_updated["container"] = false;
	this->instance_updated["container_plane"] = false;

	this->timer.tick();
	this->drop_blocks();
	this->queue_blocks(0.5f);
	this->update_gameobjects(dt);
	this->update_lava(dt);

	for (std::unordered_map<std::string, bool>::iterator iter = this->instance_updated.begin(); iter != this->instance_updated.end(); ++iter)
	{
		std::string instance_name = iter->first;
		InstancedModel::get(instance_name)->init_instances();
	}

	this->time_survived = this->timer.get_total_time_elapsed();
	this->time_survived_text.text = "Time Survived: " + Utility::float_to_str(this->time_survived, 2) + "s";
}

void Level::queue_blocks(const float wait_time)
{
	while (this->queued_blocks.size() < NUM_QUEUED_BLOCKS)
	{
		int row = 0;
		int col = 0;

		do {
			row = this->random.rand_int(0, this->ROWS - 1);
			col = this->random.rand_int(0, this->COLS - 1);
		} while (this->is_tile_queued(row, col));

		this->queue_block(row, col, wait_time + this->queued_blocks.size() * QUEUE_TIME_PER_BLOCK);
	}
}

void Level::drop_blocks()
{
	bool hit = false;
	float current_time = glfwGetTime();
	std::list<QueuedBlock>::iterator block_it = this->queued_blocks.begin();
	int block_index = 0;

	while (block_it != this->queued_blocks.end())
	{
		float time_dif = block_it->landing_time - current_time;
		int index = this->get_index(block_it->row, block_it->col);

		if (time_dif <= 0)
		{
			this->height_map[index] += 1;
			this->set_tile_queued(block_it->row, block_it->col, false);

			glm::vec3 position = glm::vec3(
				block_it->row * GRID_SIZE - GRID_SIZE / 2,
				this->height_map[index] * GRID_SIZE - GRID_SIZE / 2,
				block_it->col * GRID_SIZE - GRID_SIZE / 2
			);

			if (block_it->block_instance != nullptr)
				block_it->block_instance->set_position(position);

			if (block_it->warning_instance != nullptr)
			{
				InstancedModel::get("container_plane")->remove_instance(block_it->warning_instance);
				delete block_it->warning_instance;
				this->instance_updated["container_plane"] = true;
			}

			position.y -= GRID_SIZE / 2;
			this->particles->generate(position, 200);

			// Check if block hit player
			glm::vec3 vertices[4];
			this->player.get_hitbox_vertices(Face::BOTTOM, vertices);

			for (glm::vec3 vertex : vertices)
			{
				glm::ivec3 grid_pos = this->get_grid_pos(vertex);
				if (grid_pos.x == block_it->row && grid_pos.z == block_it->col && grid_pos.y >= this->get_height(block_it->row, block_it->col))
				{
					hit = true;
					break;
				}
			}

			block_it = this->queued_blocks.erase(block_it);
		}
		else if (time_dif <= this->DROP_DURATION)
		{
			float drop_height = (this->get_height(block_it->row, block_it->col) + this->DROP_HEIGHT_OFFSET + 0.5f) * GRID_SIZE;
			if (block_it->block_instance == nullptr)
			{
				glm::vec3 position = glm::vec3(
					block_it->row * GRID_SIZE - GRID_SIZE / 2,
					drop_height,
					block_it->col * GRID_SIZE - GRID_SIZE / 2
				);
				ModelInstance* instance = new ModelInstance("container", "static", position, glm::vec3(0.0f), glm::vec3(1.0f));
				block_it->block_instance = instance;

				InstancedModel::get("container")->add_instance(block_it->block_instance);
				this->instance_updated["container"] = true;
			}
			else
			{
				ModelInstance* instance = block_it->block_instance;
				glm::vec3 pos = instance->get_position();
				float time_elapsed = this->DROP_DURATION - time_dif;
				pos.y = drop_height - this->DROP_ACCELERATION / 2 * time_elapsed * time_elapsed;
				instance->set_position(pos);
			}
			++block_it;
		}
		else
			++block_it;
		++block_index;
	}

	if (hit)
		this->terminate();
}

void Level::update_gameobjects(const float dt)
{
	player.update_direction(camera.get_front(), camera.get_right());

	std::list<GameObject*>::iterator object_it = this->gameobjects.begin();
	while (object_it != this->gameobjects.end())
	{
		GameObject* object = *object_it;
		object->update_velocity(dt);
		object->update_hitbox();
		this->check_collision(object, dt);
		object->move(dt);
		object->update();
		++object_it;
	}

	camera.set_position(player.get_position() + glm::vec3(0.0f, 2.0f, 0.0f));
	camera.update_camera_vectors();

	if (player.get_position().y - player.get_hitbox_size().y / 2 <= this->lava_instance->get_position().y)
		this->terminate();
}

glm::ivec3 Level::get_grid_pos(glm::vec3 position)
{
	return glm::ceil(glm::vec3(
		position.x / GRID_SIZE,
		position.y / GRID_SIZE,
		position.z / GRID_SIZE
	));
}

void Level::handle_key_input(GLFWwindow* window, int key, int action)
{
	if (this->game_over)
	{
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			this->restart();
		return;
	}

	this->player.handle_key_input(window, key, action);

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		this->particles->generate(this->player.get_position(), 30);
}

void Level::handle_mouse_move_input(const float dt, const double offset_x, const double offset_y)
{
	if (this->game_over)
		return;

	this->camera.handle_mouse_move_input(dt, offset_x, offset_y);
}

void Level::check_collision(GameObject *object, const float dt)
{
	glm::vec3 vel = object->get_velocity();

	if (vel.x != 0)
		this->check_x_collision(object, dt);

	this->check_y_collision(object, dt);

	if (vel.z != 0)
		this->check_z_collision(object, dt);
}

void Level::check_y_collision(GameObject *object, const float dt)
{
	glm::vec3 pos = object->get_position();
	glm::vec3 vel = object->get_velocity();

	glm::vec3 vertices[4];
	object->get_hitbox_vertices(Face::BOTTOM, vertices);

	if (vertices[0].y + vel.y * dt < 0)
	{
		pos.y = object->get_hitbox_size().y / 2 + 0.0001;
		vel.y = 0;

		object->set_position(pos);
		object->set_velocity(vel);
		object->set_in_air(false);

		return;
	}

	for (int j = 0; j < 4; ++j)
	{
		glm::vec3 vertex = vertices[j];
		vertex.y += vel.y * dt;
		glm::ivec3 grid_pos = this->get_grid_pos(vertex);
		int map_height = this->get_height(grid_pos.x, grid_pos.z);

		if (grid_pos.x < 0 || grid_pos.x >= this->ROWS || grid_pos.z < 0 || grid_pos.z >= this->COLS)
			continue;
 
		if (grid_pos.y <= map_height)
		{
			pos.y = map_height * GRID_SIZE + object->get_hitbox_size().y / 2 + 0.0001;
			vel.y = 0;

			object->set_position(pos);
			object->set_velocity(vel);
			object->set_in_air(false);

			return;
		}
	}

	if (!object->is_in_air())
		object->set_in_air(true);
}

void Level::check_x_collision(GameObject *object, const float dt)
{
	glm::vec3 pos = object->get_position();
	glm::vec3 vel = object->get_velocity();

	glm::vec3 vertices[4];
	if (vel.x < 0)
	{
		object->get_hitbox_vertices(Face::LEFT, vertices);
		if (vertices[0].x + vel.x * dt < -GRID_SIZE)
		{
			pos.x = -GRID_SIZE + object->get_hitbox_size().x / 2 + 0.0001;
			vel.x = 0;

			object->set_position(pos);
			object->set_velocity(vel);

			return;
		}
	}
	else
	{
		object->get_hitbox_vertices(Face::RIGHT, vertices);
		if (vertices[0].x + vel.x * dt > (this->ROWS - 1) * GRID_SIZE)
		{
			pos.x = (this->ROWS - 1) * GRID_SIZE - object->get_hitbox_size().x / 2 - 0.0001;
			vel.x = 0;

			object->set_position(pos);
			object->set_velocity(vel);

			return;
		}
	}

	for (int j = 0; j < 4; ++j)
	{
		glm::vec3 vertex = vertices[j];
		vertex.x += vel.x * dt;
		glm::ivec3 grid_pos = this->get_grid_pos(vertex);
		int map_height = this->get_height(grid_pos.x, grid_pos.z);

		if (grid_pos.x < 0 || grid_pos.x >= this->ROWS || grid_pos.z < 0 || grid_pos.z >= this->COLS)
			continue;

		if (grid_pos.y <= map_height)
		{
			if (vel.x < 0)
				pos.x = grid_pos.x * GRID_SIZE + object->get_hitbox_size().x / 2 + 0.0001;
			else
				pos.x = (grid_pos.x - 1) * GRID_SIZE - object->get_hitbox_size().x / 2 - 0.0001;

			vel.x = 0;

			object->set_position(pos);
			object->set_velocity(vel);

			break;
		}
	}
}

void Level::check_z_collision(GameObject *object, const float dt)
{
	glm::vec3 pos = object->get_position();
	glm::vec3 vel = object->get_velocity();

	glm::vec3 vertices[4];
	if (vel.z < 0)
	{
		object->get_hitbox_vertices(Face::BACK, vertices);
		if (vertices[0].z + vel.z * dt < -GRID_SIZE)
		{
			pos.z = -GRID_SIZE + object->get_hitbox_size().z / 2 + 0.0001;
			vel.z = 0;

			object->set_position(pos);
			object->set_velocity(vel);

			return;
		}
	}
	else
	{
		object->get_hitbox_vertices(Face::FRONT, vertices);
		if (vertices[0].z + vel.z * dt > (this->COLS - 1) * GRID_SIZE)
		{
			pos.z = (this->COLS - 1) * GRID_SIZE - object->get_hitbox_size().z / 2 - 0.0001;
			vel.z = 0;

			object->set_position(pos);
			object->set_velocity(vel);

			return;
		}
	}

	for (int j = 0; j < 4; ++j)
	{
		glm::vec3 vertex = vertices[j];
		vertex.z += vel.z * dt;
		glm::ivec3 grid_pos = this->get_grid_pos(vertex);
		int map_height = this->get_height(grid_pos.x, grid_pos.z);

		if (grid_pos.x < 0 || grid_pos.x >= this->ROWS || grid_pos.z < 0 || grid_pos.z >= this->COLS)
			continue;

		if (grid_pos.y <= map_height)
		{
			if (vel.z < 0)
				pos.z = grid_pos.z * GRID_SIZE + object->get_hitbox_size().z / 2 + 0.0001;
			else
				pos.z = (grid_pos.z - 1) * GRID_SIZE - object->get_hitbox_size().z / 2 - 0.0001;

			vel.z = 0;

			object->set_position(pos);
			object->set_velocity(vel);

			break;
		}
	}
}

void Level::add_gameobject(GameObject* object)
{
	this->gameobjects.push_back(object);
	ModelRenderQueue::get("static")->add_instance(object->get_model_instance());
}

void Level::terminate()
{
	this->game_over = true;
	this->time_survived = this->timer.get_total_time_elapsed();
	this->particles->generate(this->player.get_position(), 100);

	this->title_text.enabled = true;
	this->title_text.text = "Game Over";
	this->title_text.x = Font::get(this->title_text.font)->get_center_x(title_text.text, 1.0f, 0.0f, 1270.0f);
	this->countdown_text.enabled = false;
}

void Level::restart()
{
	int num_tiles = this->ROWS * this->COLS;
	for (int i = 0; i < num_tiles; ++i)
	{
		this->height_map[i] = 0;
		this->queue_map[i] = false;
	}

	this->queued_blocks.clear();

	for (GameObject* object : this->gameobjects)
		if (object != &this->player)
			delete object;

	this->particles->clear();
	this->gameobjects.clear();
	this->gameobjects.push_back(&this->player);

	this->player.reset(glm::vec3(this->ROWS * GRID_SIZE / 2, 5.0f, this->COLS * GRID_SIZE / 2));
	this->camera.set_front(glm::vec3(0.0f, 0.0f, 1.0f));
	this->lava_instance->set_position(glm::vec3(this->ROWS * GRID_SIZE / 2 - GRID_SIZE, -10.0f, this->COLS * GRID_SIZE / 2 - GRID_SIZE));
	
	this->timer.reset();
	this->has_lava = false;
	this->game_over = false;

	this->title_text.enabled = true;
	this->title_text.text = "Lava rises in";
	this->title_text.x = Font::get(this->title_text.font)->get_center_x(title_text.text, 1.0f, 0.0f, 1270.0f);
	this->countdown_text.enabled = true;

	InstancedModel::get("container")->clear_instances();
	InstancedModel::get("container_plane")->clear_instances();
}

void Level::update_lava(const float dt)
{
	if (this->timer.get_ticks() == 10 && !this->has_lava)
	{
		this->lava_instance->set_position(glm::vec3(this->ROWS * GRID_SIZE / 2 - GRID_SIZE, 0.1f, this->COLS * GRID_SIZE / 2 - GRID_SIZE));
		this->has_lava = true;

		this->title_text.enabled = false;
		this->countdown_text.enabled = false;
	}
	else
	{
		this->countdown_text.text = std::to_string(10 - this->timer.get_ticks());
		this->countdown_text.x = Font::get(this->countdown_text.font)->get_center_x(countdown_text.text, 1.0f, 0.0f, 1270.0f);
	}

	if (this->has_lava)
	{
		glm::vec3 pos = this->lava_instance->get_position();
		pos.y += this->lava_speed * dt;
		this->lava_instance->set_position(pos);
	}
}

void Level::render_particles(Shader* fragment_shader)
{
	this->particles->render(fragment_shader);
}

void Level::render_lava(Shader* vertex_shader, Shader* fragment_shader)
{
	this->lava_instance->render(vertex_shader, fragment_shader);
}