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

	this->FOV = 90.0f;
	this->near_plane = 0.1f;
	this->far_plane = 1000.0f;

	this->dt = 0.0f;
	this->cur_time = 0.0f;
	this->last_time = 0.0f;

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

	this->init_fonts();
	std::cout << "Initialized Fonts" << std::endl;

	this->init_shaders();
	std::cout << "Initialized Shaders" << std::endl;

	this->init_models();
	std::cout << "Initialized Models" << std::endl;

	this->init_level();
	std::cout << "Initialized Level" << std::endl;

	this->init_lights();
	std::cout << "Initialized Lights" << std::endl;

	this->init_uniforms();
	std::cout << "Initialized Uniforms" << std::endl;
}

Game::~Game()
{
	//delete this->multisample_FBO;
	delete this->HDR_FBO;
	delete this->screen_FBO;
	glfwDestroyWindow(window);
	glfwTerminate();

	for (std::size_t i = 0; i < this->dir_lights.size(); ++i)
		delete this->dir_lights[i];

	for (std::size_t i = 0; i < this->point_lights.size(); ++i)
		delete this->point_lights[i];

	for (std::size_t i = 0; i < this->spot_lights.size(); ++i)
		delete this->spot_lights[i];
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

	game->level->handle_key_input(window, key, action);
}

void Game::scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
}

void Game::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
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
}

void Game::init_framebuffers()
{
	//this->multisample_FBO = new MultiSampleFramebuffer(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, 4);
	this->HDR_FBO = new HDRFramebuffer(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
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

	this->cube_mesh_test = new Cube3D();
}

void Game::init_matrices()
{	
	glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &this->frame_buffer_height);
	this->projection_matrix = glm::mat4(1.0f);
	this->projection_matrix = glm::perspective(glm::radians(this->FOV), (float)this->frame_buffer_width / (float)this->frame_buffer_height, this->near_plane, this->far_plane);
}

void Game::init_fonts()
{
	Font::load("game_body", "arial", 24, glm::vec4(1.0f));
	Font::load("game_title", "arial", 48, glm::vec4(1.0f));
}

void Game::init_shaders()
{
	// INIT SHADERS

	Shader* static_vertex_shader = Shader::load("static_vertex", GL_VERTEX_SHADER, "src/shaders/game/vertex.glsl");
	Shader* animated_vertex_shader = Shader::load("animated_vertex", GL_VERTEX_SHADER, "src/shaders/animation/vertex.glsl");
	Shader* instanced_vertex_shader = Shader::load("instanced_vertex", GL_VERTEX_SHADER, "src/shaders/game_instanced/vertex.glsl");
	Shader* foreground_animated_vertex_shader = Shader::load("foreground_animated_vertex", GL_VERTEX_SHADER, "src/shaders/foreground_animation/vertex.glsl");
	Shader* game_fragment_shader = Shader::load("game_fragment", GL_FRAGMENT_SHADER, "src/shaders/game/fragment.glsl");
	Shader* image_vertex_shader = Shader::load("image_vertex", GL_VERTEX_SHADER, "src/shaders/image/vertex.glsl");
	Shader* image_fragment_shader = Shader::load("image_fragment", GL_FRAGMENT_SHADER, "src/shaders/image/fragment.glsl");
	Shader* text_fragment_shader = Shader::load("text_fragment", GL_FRAGMENT_SHADER, "src/shaders/text/fragment.glsl");
	Shader* flowmap_fragment_shader = Shader::load("flowmap_fragment", GL_FRAGMENT_SHADER, "src/shaders/flowmap_3d/fragment.glsl");


	GLenum types[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	std::string screen_srcs[] = { "src/shaders/screen/vertex.glsl", "src/shaders/screen/fragment.glsl" };
	Shader::load("screen", 2, types, screen_srcs);

	std::string skybox_srcs[] = { "src/shaders/skybox/vertex.glsl", "src/shaders/skybox/fragment.glsl" };
	Shader::load("skybox", 2, types, skybox_srcs);

	GLenum depth_cube_types[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	std::string depth_cube_srcs[] = { "src/shaders/depth_cube/vertex.glsl", "src/shaders/depth_cube/geometry.glsl", "src/shaders/depth_cube/fragment.glsl" };
	Shader::load("depth_cube", 3, depth_cube_types, depth_cube_srcs);

	std::string line_srcs[] = { "src/shaders/line/vertex.glsl", "src/shaders/line/fragment.glsl" };
	Shader::load("line", 2, types, line_srcs);

	std::string cube_srcs[] = { "src/shaders/cube/vertex.glsl", "src/shaders/line/fragment.glsl" };
	Shader::load("cube", 2, types, cube_srcs);

	std::string particles_srcs[] = { "src/shaders/particles/vertex.glsl", "src/shaders/particles/fragment.glsl" };
	Shader::load("particles", 2, types, particles_srcs);

	std::string animated_particles_srcs[] = { "src/shaders/animated_particles/vertex.glsl", "src/shaders/animated_particles/fragment.glsl" };
	Shader::load("animated_particles", 2, types, animated_particles_srcs);

	// INIT PIPELINES

	GLbitfield pipeline_stages[] = { GL_VERTEX_SHADER_BIT, GL_FRAGMENT_SHADER_BIT };
	
	Shader* static_pipeline_shaders[] = { static_vertex_shader, game_fragment_shader };
	Shader* animated_pipeline_shaders[] = { animated_vertex_shader, game_fragment_shader };
	Shader* instanced_pipeline_shaders[] = { instanced_vertex_shader, game_fragment_shader };
	Shader* foreground_animated_pipeline_shaders[] = { foreground_animated_vertex_shader, game_fragment_shader };
	Shader* image_pipeline_shaders[] = { image_vertex_shader, image_fragment_shader };
	Shader* text_pipeline_shaders[] = { image_vertex_shader, text_fragment_shader };
	Shader* flowmap_pipeline_shaders[] = { static_vertex_shader, flowmap_fragment_shader };
	ShaderPipeline::load("static_game", 2, pipeline_stages, static_pipeline_shaders);
	ShaderPipeline::load("animated_game", 2, pipeline_stages, animated_pipeline_shaders);
	ShaderPipeline::load("instanced_game", 2, pipeline_stages, instanced_pipeline_shaders);
	ShaderPipeline::load("foreground_animated_game", 2, pipeline_stages, foreground_animated_pipeline_shaders);
	ShaderPipeline::load("image", 2, pipeline_stages, image_pipeline_shaders);
	ShaderPipeline::load("text", 2, pipeline_stages, text_pipeline_shaders);
	ShaderPipeline::load("flowmap", 2, pipeline_stages, flowmap_pipeline_shaders);

	ModelRenderQueue::load("static", ShaderPipeline::get("static_game"));
	ModelRenderQueue::load("animated", ShaderPipeline::get("animated_game"));
	ModelRenderQueue::load("instanced", ShaderPipeline::get("instanced_game"));
	ModelRenderQueue::load("foreground_animated", ShaderPipeline::get("foreground_animated_game"));
	TextRenderQueue::load("game_text", ShaderPipeline::get("text"));
}

void Game::init_lights()
{
	PointLight* point_light1 = new PointLight(glm::vec3(0.0f), glm::vec3(200.0f), glm::vec3(0.0f), glm::vec3(9.0f, 5.0f, 9.0f), 0.0f, 0.0f, 1.0f);
	this->point_lights.push_back(point_light1);

	DirLight* dir_light = new DirLight(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(-0.2f, -1.0f, -0.3f));
	this->dir_lights.push_back(dir_light);
}

void Game::init_models()
{
	Model::load("res/models/concrete_plane/concrete_plane.obj");

	InstancedModel::load("res/models/container/container.obj");
	InstancedModel::load("res/models/container_plane/container_plane.obj");

	crosshair = new Image(
		"res/images/crosshair_white.png",
		this->WINDOW_WIDTH / 2 - 20 / 2,
		this->WINDOW_HEIGHT / 2 - 20 / 2,
		20,
		20
	);
}

void Game::init_level()
{
	this->level = new Level(10, 10, 10);
}

void Game::init_uniforms()
{
	// TEXT

	glm::mat4 flat_proj = glm::ortho(0.0f, (float)this->WINDOW_WIDTH, 0.0f, (float)this->WINDOW_HEIGHT);
	Shader::get("image_vertex")->set_mat_4fv(flat_proj, "projection", GL_FALSE);

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
	Shader* depth_cube_shader = Shader::get("depth_cube");
	for (unsigned int i = 0; i < 6; ++i) {
		std::string arr_name = "shadow_transforms[" + std::to_string(i) + "]";
		depth_cube_shader->set_mat_4fv(shadow_transforms[i], arr_name.c_str(), GL_FALSE);
	}
	depth_cube_shader->set_vec_3f(light_pos, "light_pos");
	depth_cube_shader->set_1f(far, "far_plane");

	// SKYBOX

	Shader::get("skybox")->set_1i(this->skybox_texture->get_id(), "skybox_texture");

	// AFTER EFFECTS

	Shader* screen_shader = Shader::get("screen");
	screen_shader->set_1i(0, "screen_texture");
	screen_shader->set_1i(1, "depth_texture");
	screen_shader->set_1i(NONE, "filter_mode");
	screen_shader->set_1i(0, "show_depth");

	// MAIN FRAGMENT

	Shader* game_fragment_shader = Shader::get("game_fragment");
	game_fragment_shader->set_1f(64.0f, "material.shininess");
	game_fragment_shader->set_1i(this->depth_cube_FBO->get_texture(), "shadow_cube");
	game_fragment_shader->set_1f(far, "far_plane");
	game_fragment_shader->set_1f(0.8f, "exposure");

	// UNIFORM BUFFER

	glGenBuffers(1, &this->uniform_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer);

	Shader::get("cube")->set_vec_3f(glm::vec3(1.0f, 0.0f, 0.0f), "color");
}

void Game::update_uniforms()
{
	Camera camera = this->level->get_camera();

	// MATRICES
	glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &this->frame_buffer_height);
	this->projection_matrix = glm::perspective(glm::radians(this->FOV), (float)this->frame_buffer_width / (float)this->frame_buffer_height, this->near_plane, this->far_plane);

	glm::mat4 view_matrix_no_translate = glm::mat4(glm::mat3(camera.get_view_matrix()));
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(this->projection_matrix));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.get_view_matrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view_matrix_no_translate));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Shader* game_fragment_shader = Shader::get("game_fragment");
	game_fragment_shader->set_vec_3f(camera.get_position(), "camera_pos");

	// LIGHTS

	game_fragment_shader->set_1i(dir_lights.size(), "num_dir_lights");
	game_fragment_shader->set_1i(point_lights.size(), "num_point_lights");
	game_fragment_shader->set_1i(spot_lights.size(), "num_spot_lights");

	if (spot_lights.size())
	{
		this->spot_lights[0]->set_direction(camera.get_front());
		this->spot_lights[0]->set_position(camera.get_position());
	}

	for (std::size_t i = 0; i < this->dir_lights.size(); ++i)
		this->dir_lights[i]->send_to_shader(game_fragment_shader, i);

	for (std::size_t i = 0; i < this->point_lights.size(); ++i)
		this->point_lights[i]->send_to_shader(game_fragment_shader, i);
		
	for (std::size_t i = 0; i < this->spot_lights.size(); ++i)
		this->spot_lights[i]->send_to_shader(game_fragment_shader, i);

	Shader::get("screen")->set_1i(this->show_depth, "show_depth");
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

	this->level->handle_mouse_move_input(this->dt, this->mouse_offset_x, this->mouse_offset_y);
}

void Game::update()
{
	// UPDATE INPUT
	glfwPollEvents();

	this->update_dt();
	this->update_mouse_input();

	this->level->update(this->dt);
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

void Game::render_shadow_map()
{
	this->depth_cube_FBO->bind(true);
	glEnable(GL_DEPTH_TEST);
	ModelRenderQueue::get("static")->set_main_shader(Shader::get("depth_cube"));
	ModelRenderQueue::get("static")->render(this->dt);
}

void Game::render_level()
{
	// LAVA
	Shader::unuse();
	ShaderPipeline::get("flowmap")->use();
	this->level->render_lava(Shader::get("static_vertex"), Shader::get("flowmap_fragment"));

	// STATIC OBJECTS
	ModelRenderQueue::get("static")->set_main_shader(nullptr);
	ModelRenderQueue::get("static")->render(this->dt);

	// ANIMATED OBJECTS
	ModelRenderQueue::get("animated")->render(this->dt);

	// INSTANCED OBJECTS
	ShaderPipeline* instanced_pipeline = ShaderPipeline::get("instanced_game");
	Shader* vertex = instanced_pipeline->get_staged_shader(GL_VERTEX_SHADER_BIT);
	Shader* fragment = instanced_pipeline->get_staged_shader(GL_FRAGMENT_SHADER_BIT);
	Shader::unuse();
	instanced_pipeline->use();
	InstancedModel::get("container")->render(vertex, fragment);
	InstancedModel::get("container_plane")->render(vertex, fragment);

	// PARTICLES
	Shader::get("animated_particles")->use();
	glDepthMask(GL_FALSE);
	this->level->render_particles(Shader::get("animated_particles"));
	glDepthMask(GL_TRUE);
}

void Game::render_screen()
{
	glDisable(GL_DEPTH_TEST);

	Shader::get("screen")->use();
	glBindVertexArray(this->screen_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->screen_FBO->get_texture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->depth_FBO->get_texture());
	glDrawArrays(GL_TRIANGLES, 0, 6);

	Shader::unuse();
}

void Game::render_text()
{
	TextRenderQueue::get("game_text")->render(this->dt);
}

void Game::render()
{
	this->update_uniforms();

	// RENDER SHADOW MAP
	this->render_shadow_map();

	// SWITCH TO MULTI-SAMPLE FRAMEBUFFER
	//this->multisample_FBO->bind(true);
	this->HDR_FBO->bind(true);
	glActiveTexture(GL_TEXTURE0 + this->depth_cube_FBO->get_texture());
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->depth_cube_FBO->get_texture());

	// RENDER IN GAME SCENE
	this->render_level();

	// RENDER FOREGROUND
	glClear(GL_DEPTH_BUFFER_BIT);
	ModelRenderQueue::get("foreground_animated")->render(this->dt);

	// RENDER SCREEN
	//this->multisample_FBO->blit(this->screen_FBO, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//this->multisample_FBO->bind_default(true);
	this->HDR_FBO->blit(this->screen_FBO, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	Framebuffer::bind_default(true);
	this->render_screen();

	// RENDER TEXT
	this->render_text();

	// RENDER CROSSHAIR
	ShaderPipeline::get("image")->use();
	this->crosshair->render(Shader::get("image_vertex"), Shader::get("image_fragment"));

	// CLEAN UP
	glfwSwapBuffers(this->window);
	glFlush();

	glActiveTexture(GL_TEXTURE0);
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