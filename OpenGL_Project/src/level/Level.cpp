#include "Level.h"

// TODO implement power ups

glm::ivec3 global::over_bound = glm::ivec3(0, 0, 0);
glm::ivec3 global::collision = glm::ivec3(0, 0, 0);

Level::Level(const int rows, const int cols, const int height, std::string queue)
	:
	ROWS(rows), COLS(cols), MAX_HEIGHT(height),
	player(glm::vec3(rows * GRID_SIZE / 2, 5.0f, cols * GRID_SIZE / 2)),
	camera(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
	int num_tiles = rows * cols;
	this->height_map = new unsigned int[num_tiles];
	this->render_queue = queue;

	for (int i = 0; i < num_tiles; ++i)
		this->height_map[i] = 0;

	this->gameobjects.push_back(&this->player);
}

Level::~Level()
{
	delete[] this->height_map;
	for (GameObject* object : this->gameobjects)
		delete object;
}

void Level::queue_block(int row, int col, float time_til_landing)
{
	QueuedBlock block { row, col, glfwGetTime() + time_til_landing };
	this->queued_blocks.push_back(block);
}

void Level::update(const float dt)
{
	//camera.set_position(player.get_position() + glm::vec3(0.0f, 2.0f, 0.0f));
	//camera.update_camera_vectors();
	player.update_direction(camera.get_front(), camera.get_right());

	std::list<QueuedBlock>::iterator block_it = this->queued_blocks.begin();
	while (block_it != this->queued_blocks.end())
	{
		if (glfwGetTime() >= block_it->landing_time)
		{
			int index = this->get_index(block_it->row, block_it->col);
			this->height_map[index] += 1;

			glm::vec3 position = glm::vec3(
				block_it->row * GRID_SIZE - GRID_SIZE / 2,
				this->height_map[index] * GRID_SIZE - GRID_SIZE / 2,
				block_it->col * GRID_SIZE - GRID_SIZE / 2
			);
			ModelInstance* box_instance = new ModelInstance("container", this->render_queue, position, glm::vec3(0.0f), glm::vec3(1.0f));
			RenderQueue::get(this->render_queue)->add_instance(box_instance);

			block_it = this->queued_blocks.erase(block_it);
		}
		else
			++block_it;
	}

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
	this->player.handle_key_input(window, key, action);
}

void Level::handle_mouse_move_input(const float dt, const double offset_x, const double offset_y)
{
	this->camera.handle_mouse_move_input(dt, offset_x, offset_y);
}

void Level::check_collision(GameObject *object, const float dt)
{
	global::over_bound = glm::ivec3(0, 0, 0);
	global::collision = glm::ivec3(0, 0, 0);
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

		global::over_bound.y = 1;

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

			global::collision = grid_pos;

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

			global::over_bound.x = 1;

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

			global::over_bound.x = 1;

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

			global::collision = grid_pos;

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

			global::over_bound.z = 1;

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

			global::over_bound.z = 1;

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

			global::collision = grid_pos;

			break;
		}
	}
}

void Level::add_gameobject(GameObject* object)
{
	this->gameobjects.push_back(object);
	RenderQueue::get(this->render_queue)->add_instance(object->get_model_instance());
}