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
	this->camera = new Camera(glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	this->FOV = 90.0f;
	this->near_plane = 0.1f;
	this->far_plane = 1000.0f;

	this->dt = 0.0f;
	this->cur_time = 0.0f;
	this->last_time = 0.0f;

	this->forward_movement = 0;
	this->side_movement = 0;
	this->vertical_movement = 0;

	this->last_mouse_x = 0.0f;
	this->last_mouse_y = 0.0f;
	this->mouse_x = 0.0f;
	this->mouse_y = 0.0f;
	this->mouse_offset_x = 0.0f;
	this->mouse_offset_y = 0.0f;
	this->first_mouse = true;

	this->skybox_texture = nullptr;

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
	glDeleteFramebuffers(1, &this->FBO);
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

	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
			game->vertical_movement = 1;
		else if (action == GLFW_RELEASE)
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				game->vertical_movement = -1;
			else
				game->vertical_movement = 0;
		break;

	case GLFW_KEY_LEFT_SHIFT:
		if (action == GLFW_PRESS)
			game->vertical_movement = -1;
		else if (action == GLFW_RELEASE)
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				game->vertical_movement = 1;
			else
				game->vertical_movement = 0;
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
	// SCREEN VBO
	glGenVertexArrays(1, &this->screen_VAO);
	glBindVertexArray(this->screen_VAO);

	float screen_vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	GLuint screen_VBO;
	glGenBuffers(1, &screen_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, screen_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screen_vertices), &screen_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// FRAMEBUFFER
	glGenFramebuffers(1, &this->FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

	glGenTextures(1, &this->screen_texture);
	glBindTexture(GL_TEXTURE_2D, this->screen_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->WINDOW_WIDTH, this->WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &this->RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->screen_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FRAMEBUFFER INIT FAILED" << std::endl;
		glfwTerminate();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// SKYBOX
	std::vector<std::string> img_names{ "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };
	this->skybox_texture = new TextureCube(img_names, "Images/Skybox");

	float skybox_vertices[] = {
		// positions          
		-100.0f,  100.0f, -100.0f,
		-100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		-100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f
	};

	glGenVertexArrays(1, &this->skybox_VAO);
	glBindVertexArray(this->skybox_VAO);

	GLuint skybox_VBO;
	glGenBuffers(1, &skybox_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	
	// UNIFORM BUFFER

	glGenBuffers(1, &this->uniform_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer);

	// INSTANCE VAO
	glm::vec3 translations[10];
	for (std::size_t i = 0; i < 10; ++i)
		translations[i] = glm::vec3(i * 5, 0.0, 0.0);

	glGenBuffers(1, &this->instance_VAO);
	glBindBuffer(GL_VERTEX_ARRAY, this->instance_VAO);
	glBufferData(GL_VERTEX_ARRAY, sizeof(glm::vec3) * 10, &translations[0], GL_STATIC_DRAW);
	glEnableVertexArrayAttrib(this->instance_VAO, 5);
	//glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, )
	

	// CULL FANCE
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	// BLEND
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// DEPTH TEST
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_EQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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
	this->shaders.push_back(new Shader("vertex_shader.glsl", "fragment_shader.glsl", "geometry_shader.glsl", this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
	this->shaders.push_back(new Shader("vertex_shader.glsl", "lamp_fragment_shader.glsl", "", this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
	this->shaders.push_back(new Shader("screen_vertex_shader.glsl", "screen_fragment_shader.glsl", "", this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
	this->shaders.push_back(new Shader("skybox_vertex_shader.glsl", "skybox_fragment_shader.glsl", "", this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
}

void Game::init_lights()
{
	//SpotLight* spot_light = new SpotLight(glm::vec3(0.0f), glm::vec3(6.0f), glm::vec3(10.0f), this->camera->get_position(), this->camera->get_front(), 1.0f, 0.7f, 1.8f, 15.0f, 30.0f);
	//this->spot_lights.push_back(spot_light);

	//PointLight* point_light = new PointLight(glm::vec3(0.3f), glm::vec3(5.0f), glm::vec3(10.0f), glm::vec3(0.0f, 2.0f, 0.0f), 1.0f, 0.7f, 1.8f);
	//this->point_lights.push_back(point_light);

	DirLight* dir_light = new DirLight(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(-0.2f, -1.0f, -0.3f));
	this->dir_lights.push_back(dir_light);
}

void Game::init_models()
{
	Model* grass_plane = new Model("Models/grass_plane/grass_plane.obj");
	grass_plane->add_instance(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(3.0f));
	
	Model* box = new Model("Models/container/container.obj");
	box->add_instance(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	box->add_instance(glm::vec3(5.0f, -1.0f, 5.0f), glm::vec3(0.0f), glm::vec3(1.0f));

	Model* glass = new Model("Models/glass_pane/glass_pane.obj");
	glass->add_instance(glm::vec3(0.0f, 0.0f, 1.01f), glm::vec3(0.0f), glm::vec3(1.0f));
	glass->add_instance(glm::vec3(3.99f, 0.0f, 5.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.0f));
	glass->add_instance(glm::vec3(3.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f));

	this->models.push_back(grass_plane);
	this->models.push_back(box);
	this->transparent_models.push_back(glass);

	for (Model* model : this->models)
		model->init_instances();
	
	for (Model* model : this->transparent_models)
		model->init_instances();
}

void Game::init_uniforms()
{
	this->shaders[3]->set_1i(this->skybox_texture->get_id(), "skybox_texture");

	this->shaders[2]->set_1i(0, "screen_texture");
	this->shaders[2]->set_1i(NONE, "filter_mode");

	this->shaders[0]->set_1f(64.0f, "material.shininess");
}

void Game::update_uniforms()
{
	// MATRICES

	glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &this->frame_buffer_height);
	this->projection_matrix = glm::perspective(
		glm::radians(this->FOV),
		(float)this->frame_buffer_width / (float)this->frame_buffer_height,
		this->near_plane,
		this->far_plane
	);

	glm::mat4 view_matrix_no_translate = glm::mat4(glm::mat3(this->camera->get_view_matrix()));
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(this->projection_matrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(this->camera->get_view_matrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view_matrix_no_translate));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	this->shaders[0]->set_vec_3f(this->camera->get_position(), "camera_pos");

	// LIGHTS

	this->shaders[0]->set_1i(dir_lights.size(), "num_dir_lights");
	this->shaders[0]->set_1i(point_lights.size(), "num_point_lights");
	this->shaders[0]->set_1i(spot_lights.size(), "num_spot_lights");

	if (spot_lights.size())
	{
		this->spot_lights[0]->set_direction(this->camera->get_front());
		this->spot_lights[0]->set_position(this->camera->get_position());
	}

	for (std::size_t i = 0; i < this->dir_lights.size(); ++i)
		this->dir_lights[i]->send_to_shader(shaders[0], i);

	for (std::size_t i = 0; i < this->point_lights.size(); ++i)
		this->point_lights[i]->send_to_shader(shaders[0], i);

	for (std::size_t i = 0; i < this->spot_lights.size(); ++i)
		this->spot_lights[i]->send_to_shader(shaders[0], i);

	this->shaders[0]->set_1f(static_cast<float>(glfwGetTime()), "time");
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
	this->camera->update_keyboard_input(this->dt, this->forward_movement, this->side_movement, this->vertical_movement);
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
		model->update(this->dt);
	}

	glm::vec3 cam_position = this->camera->get_position();

	//std::sort(transparent_models.begin(), transparent_models.end(),
	//	[&cam_position](const Model* obj1, const Model* obj2) {
	//		return glm::length(cam_position - obj1->get_position()) > glm::length(cam_position - obj2->get_position());
	//	}
	//);
}

void Game::render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	/////////////////////////////START DRAW/////////////////////////////
	this->shaders[3]->use();
	glBindVertexArray(this->skybox_VAO);
	this->skybox_texture->bind();
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	this->skybox_texture->unbind();
	this->shaders[3]->unuse();
	glDepthFunc(GL_LESS);


	this->shaders[0]->use();

	for (Model* model : this->models)
		model->render(shaders[0]);

	glDepthMask(false);
	glDisable(GL_CULL_FACE);
	for (Model* model : this->transparent_models)
		model->render(shaders[0]);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);

	this->shaders[0]->unuse();
	//////////////////////////////END DRAW//////////////////////////////

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);

	this->shaders[2]->use();
	glBindVertexArray(this->screen_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->screen_texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	this->shaders[2]->unuse();


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
