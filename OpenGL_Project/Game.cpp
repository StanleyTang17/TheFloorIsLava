#pragma once

#include"Game.h"

// TODO: test point shadow in closed room
// TODO: add PCF

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

	this->show_depth = 0;

	this->init_GLFW();
	std::cout << "Initialized GLFW" << std::endl;

	this->init_window(title, resizable);
	std::cout << "Initialized Window" << std::endl;

	this->init_GLEW();
	std::cout << "Initialized GLEW" << std::endl;

	this->init_OpenGL_options();
	std::cout << "Initialized OpenGL" << std::endl;

	this->init_framebuffers();
	std::cout << "Initialized Framebuffers" << std::endl;

	this->init_others();
	std::cout << "Initialized Others" << std::endl;

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
	delete this->multisample_FBO;
	delete this->screen_FBO;
	glfwDestroyWindow(window);
	glfwTerminate();

	for (std::size_t i = 0; i < this->shaders.size(); ++i)
		delete this->shaders[i];

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
	case GLFW_KEY_C:
		if (action == GLFW_PRESS)
			game->show_depth = 1;
		else if (action == GLFW_RELEASE)
			game->show_depth = 0;
		break;

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
	// CULL FACE
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

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

void Game::init_framebuffers()
{
	this->multisample_FBO = new MultiSampleFramebuffer(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, 4);
	this->screen_FBO = new ScreenFramebuffer(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	this->depth_FBO = new DepthFramebuffer(1024, 1024);
	this->depth_cube_FBO = new DepthCubeFramebuffer(1024, 1024);
}

void Game::init_others()
{
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
	this->shaders.push_back(new Shader("screen_vertex_shader.glsl", "screen_fragment_shader.glsl", "", this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
	this->shaders.push_back(new Shader("skybox_vertex_shader.glsl", "skybox_fragment_shader.glsl", "", this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
	this->shaders.push_back(new Shader("depth_cube_vertex_shader.glsl", "depth_cube_fragment_shader.glsl", "depth_cube_geometry_shader.glsl", this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
}

void Game::init_lights()
{
	//SpotLight* spot_light = new SpotLight(glm::vec3(0.0f), glm::vec3(6.0f), glm::vec3(0.5f), this->camera->get_position(), this->camera->get_front(), 1.0f, 0.28f, 0.06f, 15.0f, 45.0f);
	//this->spot_lights.push_back(spot_light);

	//PointLight* point_light = new PointLight(glm::vec3(0.0f), glm::vec3(1.5f), glm::vec3(0.5f), glm::vec3(0.0f, 2.0f, 0.0f), 1.0f, 0.28f, 0.06f);
	PointLight* point_light2 = new PointLight(glm::vec3(2.0f), glm::vec3(2.0f), glm::vec3(0.5f), glm::vec3(4.0f, 5.0f, 4.0f), 1.0f, 0.045f, 0.006f);
	//this->point_lights.push_back(point_light);
	this->point_lights.push_back(point_light2);

	//DirLight* dir_light = new DirLight(glm::vec3(0.5f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(-0.2f, -1.0f, -0.3f));
	//this->dir_lights.push_back(dir_light);
}

void Game::init_models()
{
	Model* grass_plane = new Model("Models/grass_plane/grass_plane.obj");
	grass_plane->add_instance(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(3.0f));
	
	Model* box = new Model("Models/container/container.obj");
	box->add_instance(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	box->add_instance(glm::vec3(5.0f, -1.0f, 5.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	box->add_instance(glm::vec3(0.0f, 4.0f, 6.0f), glm::vec3(45.0f, 30.0f, 45.0f), glm::vec3(1.0f));

	Model* glass = new Model("Models/glass_pane/glass_pane.obj");
	glass->add_instance(glm::vec3(0.0f, 0.0f, 1.01f), glm::vec3(0.0f), glm::vec3(1.0f));
	glass->add_instance(glm::vec3(3.99f, 0.0f, 5.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.0f));
	glass->add_instance(glm::vec3(3.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	glass->add_instance(glm::vec3(-2.99f, 0.0f, 5.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.0f));

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
	// SHADOWS

	float light_near_plane = 1.0f, light_far_plane = 10.5f;
	glm::mat4 light_projection_matrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, light_near_plane, light_far_plane);
	glm::mat4 light_view_matrix = glm::lookAt(this->point_lights[0]->get_position(), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->light_space_matrix = light_projection_matrix * light_view_matrix;

	// POINT SHADOW

	float aspect = (float)depth_cube_FBO->get_width() / (float)depth_cube_FBO->get_height();
	float near = 1.0f;
	float far = 25.0f;
	glm::mat4 shadow_proj = glm::perspective(glm::radians(90.0f), aspect, near, far);
	glm::vec3 light_pos = this->point_lights[0]->get_position();
	std::vector<glm::mat4> shadow_transforms;
	shadow_transforms.push_back(shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3( 0.0f, -1.0f,  0.0f)));
	shadow_transforms.push_back(shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f, -1.0f,  0.0f)));
	shadow_transforms.push_back(shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  1.0f)));
	shadow_transforms.push_back(shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3( 0.0f,  0.0f, -1.0f)));
	shadow_transforms.push_back(shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3( 0.0f, -1.0f,  0.0f)));
	shadow_transforms.push_back(shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3( 0.0f, -1.0f,  0.0f)));
	for (unsigned int i = 0; i < 6; ++i) {
		std::string arr_name = "shadow_transforms[" + std::to_string(i) + "]";
		this->shaders[4]->set_mat_4fv(shadow_transforms[i], arr_name.c_str(), GL_FALSE);
	}
	this->shaders[4]->set_vec_3f(light_pos, "light_pos");
	this->shaders[4]->set_1f(far, "far_plane");

	// SKYBOX

	this->shaders[3]->set_1i(this->skybox_texture->get_id(), "skybox_texture");

	// AFTER EFFECTS

	this->shaders[2]->set_1i(0, "screen_texture");
	this->shaders[2]->set_1i(1, "depth_texture");
	this->shaders[2]->set_1i(NONE, "filter_mode");
	this->shaders[2]->set_1i(0, "show_depth");

	// MAIN FRAGMENT

	this->shaders[0]->set_1f(64.0f, "material.shininess");
	this->shaders[0]->set_1i(this->depth_cube_FBO->get_texture(), "shadow_cube");
	this->shaders[0]->set_1f(far, "far_plane");

	// UNIFORM BUFFER

	glGenBuffers(1, &this->uniform_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer);
}

void Game::update_uniforms()
{
	// MATRICES
	glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &this->frame_buffer_height);
	this->projection_matrix = glm::perspective(glm::radians(this->FOV), (float)this->frame_buffer_width / (float)this->frame_buffer_height, this->near_plane, this->far_plane);

	glm::mat4 view_matrix_no_translate = glm::mat4(glm::mat3(this->camera->get_view_matrix()));
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(this->projection_matrix));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(this->camera->get_view_matrix()));
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

	this->shaders[2]->set_1i(this->show_depth, "show_depth");
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

void Game::render_skybox(Shader* shader)
{
	// SKYBOX
	shader->use();
	glBindVertexArray(this->skybox_VAO);
	this->skybox_texture->bind();
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	this->skybox_texture->unbind();
	shader->unuse();
	glDepthFunc(GL_LESS);
}

void Game::render_models(Shader* shader)
{
	glEnable(GL_DEPTH_TEST);

	// OBJECTS
	shader->use();

	for (Model* model : this->models)
		model->render(shader);

	glDepthMask(false);
	glDisable(GL_CULL_FACE);
	for (Model* model : this->transparent_models)
		model->render(shader);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);

	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	//model = glm::scale(model, glm::vec3(0.5f));
	//shader->set_mat_4fv(model, "model", GL_FALSE);
	//renderCube();

	shader->unuse();
}

void Game::render_screen()
{
	glDisable(GL_DEPTH_TEST);

	this->shaders[2]->use();
	glBindVertexArray(this->screen_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->screen_FBO->get_texture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->depth_FBO->get_texture());
	glDrawArrays(GL_TRIANGLES, 0, 6);

	this->shaders[2]->unuse();
}

void Game::render()
{
	this->depth_cube_FBO->bind(true);
	this->render_models(this->shaders[4]);

	this->multisample_FBO->bind(true);
	//this->render_skybox(this->shaders[3]);
	glActiveTexture(GL_TEXTURE0 + this->depth_cube_FBO->get_texture());
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->depth_cube_FBO->get_texture());
	this->render_models(this->shaders[0]);

	this->multisample_FBO->blit(this->screen_FBO, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	this->multisample_FBO->bind_default(true);

	this->render_screen();

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

void Game::renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}