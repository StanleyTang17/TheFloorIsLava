#pragma once

#include"Game.h"

Game::Game(const char* title, const int width, const int height, const int version_major, const int version_minor, GLboolean resizable)
	:
	WINDOW_WIDTH(width), WINDOW_HEIGHT(height), GL_VERSION_MAJOR(version_major), GL_VERSION_MINOR(version_minor)
{
	this->window = nullptr;
	this->frame_buffer_width = width;
	this->frame_buffer_height = height;

	this->move_dir = -1;
	this->camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	this->FOV = 90.0f;
	this->near_plane = 0.1f;
	this->far_plane = 1000.0f;

	this->dt = 0.0f;
	this->cur_time = 0.0f;
	this->last_time = 0.0f;

	this->forward_movement = 0;
	this->side_movement = 0;

	this->last_mouse_x = 0.0f;
	this->last_mouse_y = 0.0f;
	this->mouse_x = 0.0f;
	this->mouse_y = 0.0f;
	this->mouse_offset_x = 0.0f;
	this->mouse_offset_y = 0.0f;
	this->first_mouse = true;

	this->init_GLFW();
	std::cout << "Initialized GLFW" << std::endl;

	this->init_window(title, resizable);
	std::cout << "Initialized Window" << std::endl;

	this->init_GLEW();
	std::cout << "Initialized GLEW" << std::endl;

	this->init_OpenGL_options();
	std::cout << "Initialized OpenGL" << std::endl;

	this->init_matrices();
	std::cout << "Initialized Matrices" << std::endl;

	this->init_shaders();
	std::cout << "Initialized Shaders" << std::endl;

	this->init_models();
	std::cout << "Initialized Models" << std::endl;

	this->init_lights();
	std::cout << "Initialized Lights" << std::endl;

	this->init_uniforms();
	std::cout << "Initialized Uniforms" << std::endl;
}

Game::~Game()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	for (std::size_t i = 0; i < this->shaders.size(); ++i)
		delete this->shaders[i];

	for (std::size_t i = 0; i < this->light_positions.size(); ++i)
		delete this->light_positions[i];

	for (std::size_t i = 0; i < this->dir_lights.size(); ++i)
		delete this->dir_lights[i];

	for (std::size_t i = 0; i < this->point_lights.size(); ++i)
		delete this->point_lights[i];

	for (std::size_t i = 0; i < this->spot_lights.size(); ++i)
		delete this->spot_lights[i];

	delete this->camera;
}

void Game::framebuffer_resize_callback(GLFWwindow* window, int frame_buffer_width, int frame_buffer_height)
{
	glViewport(0, 0, frame_buffer_width, frame_buffer_height);
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		game->set_window_should_close(true);
		return;
	}

	switch (key)
	{
	case GLFW_KEY_W:
		if (action == GLFW_PRESS)
			game->forward_movement = 1;
		else if (action == GLFW_RELEASE)
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				game->forward_movement = -1;
			else
				game->forward_movement = 0;
		break;
	case GLFW_KEY_A:
		if (action == GLFW_PRESS)
			game->side_movement = -1;
		else if (action == GLFW_RELEASE)
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				game->side_movement = 1;
			else
				game->side_movement = 0;
		break;
	case GLFW_KEY_S:
		if (action == GLFW_PRESS)
			game->forward_movement = -1;
		else if (action == GLFW_RELEASE)
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				game->forward_movement = 1;
			else
				game->forward_movement = 0;
		break;
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
			game->side_movement = 1;
		else if (action == GLFW_RELEASE)
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				game->side_movement = -1;
			else
				game->side_movement = 0;
		break;
	default:
		break;
	}
}

void Game::init_GLFW()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "GLFW INIT FAILED!" << std::endl;
		glfwTerminate();
	}
}

void Game::init_window(const char* title, GLboolean resizable)
{
	// CREATE WINDOW
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "WINDOW INIT FAILED" << std::endl;
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &this->frame_buffer_height);
	glfwSetFramebufferSizeCallback(this->window, framebuffer_resize_callback);

	glfwMakeContextCurrent(this->window); // IMPORTANT!
}

void Game::init_GLEW()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW INIT FAILED!" << std::endl;
		glfwTerminate();
	}
}

void Game::init_OpenGL_options()
{
	// OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW); // SHAPES MADE UP OF CCW VERTICES WILL NOT DRAW!!

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(this->window, this);
	glfwSetKeyCallback(this->window, key_callback);
}

void Game::init_matrices()
{	
	glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &this->frame_buffer_height);
	this->projection_matrix = glm::mat4(1.0f);
	this->projection_matrix = glm::perspective(glm::radians(this->FOV), (float)this->frame_buffer_width / (float)this->frame_buffer_height, this->near_plane, this->far_plane);
}

void Game::init_shaders()
{
	this->shaders.push_back(new Shader("vertex_shader.glsl", "fragment_shader.glsl", "", this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
	this->shaders.push_back(new Shader("vertex_shader.glsl", "lamp_fragment_shader.glsl", "", this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
}

void Game::init_lights()
{
	SpotLight* spot_light = new SpotLight(glm::vec3(0.0f), glm::vec3(6.0f), glm::vec3(10.0f), this->camera->get_position(), this->camera->get_front(), 1.0f, 0.7f, 1.8f, 15.0f, 30.0f);
	this->spot_lights.push_back(spot_light);

	//PointLight* point_light = new PointLight(glm::vec3(0.3f), glm::vec3(5.0f), glm::vec3(10.0f), glm::vec3(1.2f, 1.0f, -0.5f), 1.0f, 0.7f, 1.8f);
	//this->point_lights.push_back(point_light);

	DirLight* dir_light = new DirLight(glm::vec3(0.2f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(-0.2f, -1.0f, -0.3f));
	this->dir_lights.push_back(dir_light);

}

void Game::init_models()
{
	this->models.push_back(new Model("Models/backpack/backpack.obj"));
}

void Game::init_uniforms()
{
	this->shaders[0]->set_mat_4fv(this->camera->get_view_matrix(), "view_matrix", GL_FALSE);
	this->shaders[0]->set_mat_4fv(this->projection_matrix, "projection_matrix", GL_FALSE);

	this->shaders[0]->set_1f(64.0f, "material.shininess");
}

void Game::update_uniforms()
{
	// MATERIALS
	//for (std::size_t i = 0; i < this->materials.size(); ++i)
	//	this->materials[i]->send_to_shader(this->shaders[0]);

	// VIEW MATRIX (CAMERA)
	this->shaders[0]->set_mat_4fv(this->camera->get_view_matrix(), "view_matrix", GL_FALSE);
	this->shaders[0]->set_vec_3f(this->camera->get_position(), "camera_pos");

	// PROJECTION MATRIX
	glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &this->frame_buffer_height);
	this->projection_matrix = glm::perspective(
		glm::radians(this->FOV),
		(float)this->frame_buffer_width / (float)this->frame_buffer_height,
		this->near_plane,
		this->far_plane
	);
	this->shaders[0]->set_mat_4fv(this->projection_matrix, "projection_matrix", GL_FALSE);

	this->shaders[1]->set_mat_4fv(this->camera->get_view_matrix(), "view_matrix", GL_FALSE);
	this->shaders[1]->set_mat_4fv(this->projection_matrix, "projection_matrix", GL_FALSE);

	this->shaders[0]->set_1i(dir_lights.size(), "num_dir_lights");
	this->shaders[0]->set_1i(point_lights.size(), "num_point_lights");
	this->shaders[0]->set_1i(spot_lights.size(), "num_spot_lights");

	this->spot_lights[0]->set_direction(this->camera->get_front());
	this->spot_lights[0]->set_position(this->camera->get_position());

	for (std::size_t i = 0; i < this->dir_lights.size(); ++i)
		this->dir_lights[i]->send_to_shader(shaders[0], i);

	for (std::size_t i = 0; i < this->point_lights.size(); ++i)
		this->point_lights[i]->send_to_shader(shaders[0], i);

	for (std::size_t i = 0; i < this->spot_lights.size(); ++i)
		this->spot_lights[i]->send_to_shader(shaders[0], i);
}

void Game::update_dt()
{
	this->cur_time = static_cast<float>(glfwGetTime());
	this->dt = this->cur_time - this->last_time;
	this->last_time = this->cur_time;
}

void Game::update_mouse_input()
{
	glfwGetCursorPos(this->window, &this->mouse_x, &this->mouse_y);

	if (this->first_mouse)
	{
		this->last_mouse_x = this->mouse_x;
		this->last_mouse_y = this->mouse_y;
		this->first_mouse = false;
	}

	this->mouse_offset_x = this->mouse_x - this->last_mouse_x;
	this->mouse_offset_y = this->last_mouse_y - this->mouse_y;

	this->last_mouse_x = this->mouse_x;
	this->last_mouse_y = this->mouse_y;

	this->camera->update_mouse_input(this->dt, this->mouse_offset_x, this->mouse_offset_y);
}

void Game::update_keyboard_input()
{
	this->camera->update_keyboard_input(this->dt, this->forward_movement, this->side_movement);
}

void Game::update()
{
	// UPDATE INPUT
	glfwPollEvents();

	this->update_dt();
	this->update_mouse_input();
	this->update_keyboard_input();

	this->update_uniforms();

	for (Model* model : this->models)
	{
		model->update();
	}
}

void Game::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->shaders[0]->use();

	for (Model* model : this->models)
	{
		model->render(shaders[0]);
	}

	this->shaders[0]->unuse();
	//////////////////////////////END DRAW//////////////////////////////

	glfwSwapBuffers(this->window);
	glFlush();

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

int Game::get_window_should_close()
{
	return glfwWindowShouldClose(this->window);
}

void Game::set_window_should_close(bool should_close)
{
	glfwSetWindowShouldClose(this->window, should_close);
}
