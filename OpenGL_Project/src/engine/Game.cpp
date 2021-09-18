#pragma once

#include"Game.h"

// TODO: test point shadow in closed room
// TODO: add PCF
// TODO: fix shakey camera

Game::Game(const char* title, const int width, const int height, const int version_major, const int version_minor, GLboolean resizable)
	:
	WINDOW_WIDTH(width), WINDOW_HEIGHT(height), GL_VERSION_MAJOR(version_major), GL_VERSION_MINOR(version_minor)
{
	this->window = nullptr;
	this->frame_buffer_width = width;
	this->frame_buffer_height = height;

	this->move_dir = -1;

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

	this->init_game_objects();
	std::cout << "Initialized GameObjects" << std::endl;

	this->init_lights();
	std::cout << "Initialized Lights" << std::endl;

	this->init_uniforms();
	std::cout << "Initialized Uniforms" << std::endl;

	this->init_fonts();
	std::cout << "Initialized Fonts" << std::endl;
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
	delete this->player;
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

	game->player->update_keyboard_input(window, key, action);
	game->crate2->get_keyboard_control()->update_input(window, key, action);
	game->crate->get_keyboard_control()->update_input(window, key, action);
}

void Game::scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
}

void Game::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	game->player->update_mouse_input(window, button, action);
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

	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(this->window, this);
	glfwSetKeyCallback(this->window, key_callback);
	glfwSetScrollCallback(this->window, scroll_callback);
	glfwSetMouseButtonCallback(this->window, mouse_button_callback);

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
	this->skybox_texture = new TextureCube(img_names, "res/images/skybox");

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

	// TEXT VAO & VBO
	glGenVertexArrays(1, &this->text_VAO);
	glBindVertexArray(this->text_VAO);

	glGenBuffers(1, &this->text_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->text_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->test_texture = new Texture2D("font", "container.png", "res/images/");
}

void Game::init_matrices()
{	
	glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &this->frame_buffer_height);
	this->projection_matrix = glm::mat4(1.0f);
	this->projection_matrix = glm::perspective(glm::radians(this->FOV), (float)this->frame_buffer_width / (float)this->frame_buffer_height, this->near_plane, this->far_plane);
}

void Game::init_shaders()
{
	this->shaders.push_back(new Shader("src/shaders/game", false, this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
	this->shaders.push_back(new Shader("src/shaders/screen", false, this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
	this->shaders.push_back(new Shader("src/shaders/skybox", false, this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
	this->shaders.push_back(new Shader("src/shaders/depth_cube", true, this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR));
	this->shaders.push_back(new Shader("src/shaders/text", false, this->GL_VERSION_MAJOR, this->GL_VERSION_MAJOR));
	this->shaders.push_back(new Shader("src/shaders/animation", false, this->GL_VERSION_MAJOR, this->GL_VERSION_MAJOR));
}

void Game::init_lights()
{
	//SpotLight* spot_light = new SpotLight(glm::vec3(0.0f), glm::vec3(6.0f), glm::vec3(0.5f), this->camera->get_position(), this->camera->get_front(), 1.0f, 0.28f, 0.06f, 15.0f, 45.0f);
	//this->spot_lights.push_back(spot_light);

	//PointLight* point_light = new PointLight(glm::vec3(0.0f), glm::vec3(1.5f), glm::vec3(0.5f), glm::vec3(0.0f, 2.0f, 0.0f), 1.0f, 0.28f, 0.06f);
	PointLight* point_light2 = new PointLight(glm::vec3(2.0f), glm::vec3(2.0f), glm::vec3(0.5f), glm::vec3(4.0f, 5.0f, 4.0f), 1.0f, 0.045f, 0.006f);
	//this->point_lights.push_back(point_light);
	this->point_lights.push_back(point_light2);

	DirLight* dir_light = new DirLight(glm::vec3(1.5f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(-0.2f, -1.0f, -0.3f));
	this->dir_lights.push_back(dir_light);
}

void Game::init_models()
{
	Model::load_model("res/models/grass_plane/grass_plane.obj");
	Model::load_model("res/models/container/container.obj");
	Model::load_model("res/models/glass_pane/glass_pane.obj");
	Model::load_model("res/models/ball/ball.obj");
	AnimatedModel::load_model("res/animations/zombie/zombie2.dae", "res/animations/zombie/split.txt");

	this->static_models.push_back(new ModelInstance("grass_plane", glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(3.0f)));
	// this->animated_models.push_back(new ModelInstance("zombie2", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(3.0f)));
}

void Game::init_game_objects()
{
	this->camera = new Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->player = new Player(glm::vec3(0.0f, 0.0f, 0.0f), this->camera);
	this->crate = new Crate(glm::vec3(3.0f, 2.5f, 6.0f), Collision::Behavior::STATIC, 2.0f);
	this->crate2 = new Crate(glm::vec3(2.0f, 0.0f, 2.0f), Collision::Behavior::KINETIC, 1.0f);
	this->crate2->set_control(new KeyboardControl(GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_ENTER, GLFW_KEY_RIGHT_SHIFT));
	
	this->add_game_object(this->player);
	this->add_game_object(this->crate);
	this->add_game_object(this->crate2);
}

void Game::init_uniforms()
{
	// TEXT

	glm::mat4 text_proj = glm::ortho(0.0f, (float)this->WINDOW_WIDTH, 0.0f, (float)this->WINDOW_HEIGHT);
	this->shaders[4]->set_mat_4fv(text_proj, "projection", GL_FALSE);

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
		this->shaders[3]->set_mat_4fv(shadow_transforms[i], arr_name.c_str(), GL_FALSE);
	}
	this->shaders[3]->set_vec_3f(light_pos, "light_pos");
	this->shaders[3]->set_1f(far, "far_plane");

	// SKYBOX

	this->shaders[2]->set_1i(this->skybox_texture->get_id(), "skybox_texture");

	// AFTER EFFECTS

	this->shaders[1]->set_1i(0, "screen_texture");
	this->shaders[1]->set_1i(1, "depth_texture");
	this->shaders[1]->set_1i(NONE, "filter_mode");
	this->shaders[1]->set_1i(0, "show_depth");

	// MAIN FRAGMENT

	this->shaders[0]->set_1f(64.0f, "material.shininess");
	this->shaders[0]->set_1i(this->depth_cube_FBO->get_texture(), "shadow_cube");
	this->shaders[0]->set_1f(far, "far_plane");

	this->shaders[5]->set_1f(64.0f, "material.shininess");
	this->shaders[5]->set_1i(this->depth_cube_FBO->get_texture(), "shadow_cube");
	this->shaders[5]->set_1f(far, "far_plane");

	// UNIFORM BUFFER

	glGenBuffers(1, &this->uniform_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer);
}

void Game::init_fonts()
{
	this->arial = new Font("arial", 24);
	this->arial_big = new Font("arial", 48);
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
	this->shaders[5]->set_vec_3f(this->camera->get_position(), "camera_pos");

	// LIGHTS

	this->shaders[0]->set_1i(dir_lights.size(), "num_dir_lights");
	this->shaders[0]->set_1i(point_lights.size(), "num_point_lights");
	this->shaders[0]->set_1i(spot_lights.size(), "num_spot_lights");

	this->shaders[5]->set_1i(dir_lights.size(), "num_dir_lights");
	this->shaders[5]->set_1i(point_lights.size(), "num_point_lights");
	this->shaders[5]->set_1i(spot_lights.size(), "num_spot_lights");

	if (spot_lights.size())
	{
		this->spot_lights[0]->set_direction(this->camera->get_front());
		this->spot_lights[0]->set_position(this->camera->get_position());
	}

	for (std::size_t i = 0; i < this->dir_lights.size(); ++i)
	{
		this->dir_lights[i]->send_to_shader(shaders[0], i);
		this->dir_lights[i]->send_to_shader(shaders[5], i);
	}

	for (std::size_t i = 0; i < this->point_lights.size(); ++i)
	{
		this->point_lights[i]->send_to_shader(shaders[0], i);
		this->point_lights[i]->send_to_shader(shaders[5], i);
	}
		
	for (std::size_t i = 0; i < this->spot_lights.size(); ++i)
	{
		this->spot_lights[i]->send_to_shader(shaders[0], i);
		this->spot_lights[i]->send_to_shader(shaders[5], i);
	}
		
	this->shaders[0]->set_1f(static_cast<float>(glfwGetTime()), "time");
	this->shaders[5]->set_1f(static_cast<float>(glfwGetTime()), "time");

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

}

void Game::update()
{
	// UPDATE INPUT
	glfwPollEvents();

	this->update_dt();
	this->update_mouse_input();
	this->update_keyboard_input();

	//this->update_uniforms();

	for (ModelInstance* instance : this->static_models)
		instance->update(this->dt);

	for (ModelInstance* instance : this->animated_models)
		instance->update(this->dt);
	
	for (std::size_t i = 0; i < game_objects.size(); ++i)
		game_objects[i]->update_velocity();

	this->hit = false;
	for (std::size_t i = 0; i < game_objects.size(); ++i)
	{
		for (std::size_t j = i + 1; j < game_objects.size(); ++j)
		{
			if (game_objects[i]->check_collision(game_objects[j], this->dt))
				this->hit = true;
		}
		game_objects[i]->move(this->dt);
		game_objects[i]->update();
	}

	//glm::vec3 cam_position = this->camera->get_position();
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

	for (ModelInstance* instance : this->static_models)
		instance->render(shader);

	glDepthMask(false);
	glDisable(GL_CULL_FACE);
	for (ModelInstance* instance : this->transparent_models)
		instance->render(shader);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);
	
	shader->unuse();
}

void Game::render_animated_models(Shader* shader)
{
	shader->use();

	for (ModelInstance* instance : this->animated_models)
		instance->render(shader);

	shader->unuse();
}

void Game::render_screen()
{
	glDisable(GL_DEPTH_TEST);

	this->shaders[1]->use();
	glBindVertexArray(this->screen_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->screen_FBO->get_texture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->depth_FBO->get_texture());
	glDrawArrays(GL_TRIANGLES, 0, 6);

	this->shaders[1]->unuse();
}

void Game::render_text(Shader* shader, Font* font, std::string text, float x, float y, float scale, glm::vec3 color)
{
	shader->set_1i(0, "font_texture");
	shader->set_vec_3f(color, "font_color");
	shader->use();
	glBindVertexArray(this->text_VAO);
	
	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
	{
		Character ch = font->get_character(*c);

		float x_pos = x + ch.bearing.x * scale;
		float y_pos = y + (ch.bearing.y - ch.size.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		float vertices[6][4] = {
			{ x_pos,     y_pos + h,   0.0f, 0.0f },
			{ x_pos,     y_pos,       0.0f, 1.0f },
			{ x_pos + w, y_pos,       1.0f, 1.0f },

			{ x_pos,     y_pos + h,   0.0f, 0.0f },
			{ x_pos + w, y_pos,       1.0f, 1.0f },
			{ x_pos + w, y_pos + h,   1.0f, 0.0f }
		};

		glBindBuffer(GL_ARRAY_BUFFER, this->text_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		ch.texture->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.advance >> 6) * scale;
	}

	shader->unuse();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Game::render()
{
	this->update_uniforms();

	this->depth_cube_FBO->bind(true);
	this->render_models(this->shaders[3]);

	this->multisample_FBO->bind(true);
	glActiveTexture(GL_TEXTURE0 + this->depth_cube_FBO->get_texture());
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->depth_cube_FBO->get_texture());
	this->render_models(this->shaders[0]);
	this->render_animated_models(this->shaders[5]);

	this->multisample_FBO->blit(this->screen_FBO, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	this->multisample_FBO->bind_default(true);

	this->render_screen();

	this->render_text(this->shaders[4], this->arial_big, "Hit: " + std::to_string(this->hit), 1150.0f, 750.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	this->render_text(this->shaders[4], this->arial, "Position: " + glm::to_string(this->player->get_position()), 0.0f, 48.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	this->render_text(this->shaders[4], this->arial, "Frame Rate: " + std::to_string((int)(1.0f / this->dt)), 0.0f, 78.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	glfwSwapBuffers(this->window);
	glFlush();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Game::add_game_object(GameObject* game_object)
{
	this->game_objects.push_back(game_object);
	ModelInstance* instance = game_object->get_model_instance();
	if (instance != nullptr)
	{
		if (instance->is_animated())
			this->animated_models.push_back(instance);
		else
			this->static_models.push_back(instance);
	}
}

void Game::remove_game_object(GameObject* game_object)
{
	ModelInstance* instance = game_object->get_model_instance();
	for(std::size_t i = 0; i < game_objects.size(); ++i)
		if (game_objects[i] == game_object)
		{
			game_objects.erase(game_objects.begin() + i);

			std::vector<ModelInstance*>* list = instance->is_animated() ? &(this->animated_models) : &(this->static_models);
			for (std::size_t j = 0; j < list->size(); ++j)
				if (list->at(j) == instance)
					list->erase(list->begin() + j);
		}
}

int Game::get_window_should_close()
{
	return glfwWindowShouldClose(this->window);
}

void Game::set_window_should_close(bool should_close)
{
	glfwSetWindowShouldClose(this->window, should_close);
}