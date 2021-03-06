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
	delete this->bloom_FBO;
	delete this->ping_pong_FBOs[0];
	delete this->ping_pong_FBOs[1];
	delete this->depth_FBO;
	delete this->depth_cube_FBO;
	delete this->screen_FBO;
	glfwDestroyWindow(window);
	glfwTerminate();
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
	this->bloom_FBO = new BloomFramebuffer(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	this->ping_pong_FBOs[0] = new HDRFramebuffer(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, false);
	this->ping_pong_FBOs[1] = new HDRFramebuffer(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, false);
	this->screen_FBO = new ScreenFramebuffer(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	this->depth_FBO = new DepthFramebuffer(1024, 1024);
	this->depth_cube_FBO = new DepthCubeFramebuffer(1024, 1024);	
}

void Game::init_others()
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
	Font::load("game_body", "impact", 24, glm::vec4(1.0f));
	Font::load("game_title", "impact", 56, glm::vec4(1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f);
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
	Shader* screen_vertex_shader = Shader::load("screen_vertex", GL_VERTEX_SHADER, "src/shaders/screen/vertex.glsl");
	Shader* screen_fragment_shader = Shader::load("screen_fragment", GL_FRAGMENT_SHADER, "src/shaders/screen/fragment.glsl");
	Shader* text_fragment_shader = Shader::load("text_fragment", GL_FRAGMENT_SHADER, "src/shaders/text/fragment.glsl");
	Shader* flowmap_fragment_shader = Shader::load("flowmap_fragment", GL_FRAGMENT_SHADER, "src/shaders/flowmap_3d/fragment.glsl");
	Shader* gaussian_blur_fragment_shader = Shader::load("gaussian_blur_fragment", GL_FRAGMENT_SHADER, "src/shaders/gaussian_blur/fragment.glsl");


	GLenum types[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

	GLenum depth_cube_types[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	std::string depth_cube_srcs[] = { "src/shaders/depth_cube/vertex.glsl", "src/shaders/depth_cube/geometry.glsl", "src/shaders/depth_cube/fragment.glsl" };
	Shader::load("depth_cube", 3, depth_cube_types, depth_cube_srcs);

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
	Shader* screen_pipeline_shaders[] = { screen_vertex_shader, screen_fragment_shader };
	Shader* gaussian_blur_pipeline_shaders[] = { screen_vertex_shader, gaussian_blur_fragment_shader };
	ShaderPipeline::load("static_game", 2, pipeline_stages, static_pipeline_shaders);
	ShaderPipeline::load("animated_game", 2, pipeline_stages, animated_pipeline_shaders);
	ShaderPipeline::load("instanced_game", 2, pipeline_stages, instanced_pipeline_shaders);
	ShaderPipeline::load("foreground_animated_game", 2, pipeline_stages, foreground_animated_pipeline_shaders);
	ShaderPipeline::load("image", 2, pipeline_stages, image_pipeline_shaders);
	ShaderPipeline::load("text", 2, pipeline_stages, text_pipeline_shaders);
	ShaderPipeline::load("flowmap", 2, pipeline_stages, flowmap_pipeline_shaders);
	ShaderPipeline::load("screen", 2, pipeline_stages, screen_pipeline_shaders);
	ShaderPipeline::load("gaussian_blur", 2, pipeline_stages, gaussian_blur_pipeline_shaders);

	ModelRenderQueue::load("static", ShaderPipeline::get("static_game"));
	ModelRenderQueue::load("animated", ShaderPipeline::get("animated_game"));
	ModelRenderQueue::load("instanced", ShaderPipeline::get("instanced_game"));
	ModelRenderQueue::load("foreground_animated", ShaderPipeline::get("foreground_animated_game"));
	TextRenderQueue::load("game_text", ShaderPipeline::get("text"));
}

void Game::init_lights()
{
	DirLight* dir_light = new DirLight(glm::vec3(0.1f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(-0.2f, -1.0f, -0.3f));
	Light::add(LightType::DIRECTION, dir_light);
}

void Game::init_models()
{
	Model::load("res/models/concrete_plane/concrete_plane.obj");

	InstancedModel::load("res/models/container/container.obj");
	InstancedModel::load("res/models/container_plane/container_plane.obj");
	InstancedModel::load("res/models/wall_light/wall_light.obj");
}

void Game::init_level()
{
	this->level = new Level(10, 10, 10);
}

void Game::init_uniforms()
{
	// IMAGE
	Shader::get("image_fragment")->set_1f(-1.0f, "alpha_override");

	// TEXT

	glm::mat4 flat_proj = glm::ortho(0.0f, (float)this->WINDOW_WIDTH, 0.0f, (float)this->WINDOW_HEIGHT);
	Shader::get("image_vertex")->set_mat_4fv(flat_proj, "projection", GL_FALSE);

	// SHADOWS

	PointLight* main_light = this->level->get_lights()[0];

	float light_near_plane = 1.0f, light_far_plane = 10.5f;
	glm::mat4 light_projection_matrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, light_near_plane, light_far_plane);
	glm::mat4 light_view_matrix = glm::lookAt(main_light->get_position(), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->light_space_matrix = light_projection_matrix * light_view_matrix;

	// POINT SHADOW

	float aspect = (float)depth_cube_FBO->get_width() / (float)depth_cube_FBO->get_height();
	float near = 1.0f;
	float far = 25.0f;
	glm::mat4 shadow_proj = glm::perspective(glm::radians(90.0f), aspect, near, far);
	glm::vec3 light_pos = main_light->get_position();
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

	// SCREEN

	Shader* screen_fragment_shader = Shader::get("screen_fragment");
	screen_fragment_shader->set_1i(0, "screen_texture");
	screen_fragment_shader->set_1i(1, "bloom_blur_texture");
	screen_fragment_shader->set_1i(2, "depth_texture");
	screen_fragment_shader->set_1i(Filter::FXAA, "filter_mode");
	screen_fragment_shader->set_1f(2.0f, "fxaa_max_range");
	screen_fragment_shader->set_1f(1.0f / 256.0f, "fxaa_reduce_min");
	screen_fragment_shader->set_1f(1.0f / 16.0f, "fxaa_reduce_multiplier");
	screen_fragment_shader->set_1f(0.8f, "exposure");
	screen_fragment_shader->set_vec_2f(glm::vec2(1.0f / this->WINDOW_WIDTH, 1.0f / this->WINDOW_HEIGHT), "inverse_screen_size");

	// MAIN FRAGMENT

	Shader* game_fragment_shader = Shader::get("game_fragment");
	game_fragment_shader->set_1f(64.0f, "material.shininess");
	game_fragment_shader->set_1i(this->depth_cube_FBO->get_texture(), "shadow_cube");
	game_fragment_shader->set_1f(far, "far_plane");
	game_fragment_shader->set_1i(false, "is_drawing_wall_light");

	// UNIFORM BUFFER

	glGenBuffers(1, &this->uniform_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer);

	Shader::get("gaussian_blur_fragment")->set_1i(0, "image_texture");

	Shader::get("flowmap_fragment")->set_vec_3f(glm::vec3(2.5f, 2.0f, 2.0f), "color_enhance");
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

	Light::send_all_to_shader(Shader::get("game_fragment"));
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

	this->update_uniforms();
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
	fragment->set_1i(true, "is_drawing_wall_light");
	InstancedModel::get("wall_light")->render(vertex, fragment);
	fragment->set_1i(false, "is_drawing_wall_light");

	// PARTICLES
	Shader::get("animated_particles")->use();
	glDepthMask(GL_FALSE);
	this->level->render_particles(Shader::get("animated_particles"));
	glDepthMask(GL_TRUE);
}

void Game::render_blur()
{
	glActiveTexture(GL_TEXTURE0);

	Shader::unuse();
	ShaderPipeline::get("gaussian_blur")->use();

	int blur_interations = 10;
	bool horizontal = true;
	bool first_iteration = true;
	for (int i = 0; i < blur_interations; ++i)
	{
		HDRFramebuffer* cur_buffer = this->ping_pong_FBOs[horizontal];
		HDRFramebuffer* next_buffer = this->ping_pong_FBOs[!horizontal];
		cur_buffer->bind(false);
		Shader::get("gaussian_blur_fragment")->set_1i(horizontal, "horizontal");
		glBindTexture(GL_TEXTURE_2D, first_iteration ? this->bloom_FBO->get_bright_texture() : next_buffer->get_texture());
		glBindVertexArray(this->screen_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		horizontal = !horizontal;
		if (first_iteration) first_iteration = false;
	}

	this->blurred_texture = this->ping_pong_FBOs[!horizontal]->get_texture();
}

void Game::render_screen()
{
	glDisable(GL_DEPTH_TEST);

	Shader::unuse();
	ShaderPipeline::get("screen")->use();
	glBindVertexArray(this->screen_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->bloom_FBO->get_texture());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->blurred_texture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->depth_FBO->get_texture());

	glDrawArrays(GL_TRIANGLES, 0, 6);

	Shader::unuse();
}

void Game::render_text()
{
	if (this->level->is_game_over())
	{
		ShaderPipeline* image_pipline = ShaderPipeline::get("image");
		Shader::unuse();
		image_pipline->use();

		this->level->render_text_bg(image_pipline->get_staged_shader(GL_VERTEX_SHADER_BIT), image_pipline->get_staged_shader(GL_FRAGMENT_SHADER_BIT));

		ShaderPipeline::unuse();
	}
	
	TextRenderQueue::get("game_text")->render(this->dt);
}

void Game::render()
{
	// RENDER SHADOW MAP
	this->render_shadow_map();

	// SWITCH TO MULTI-SAMPLE FRAMEBUFFER
	this->bloom_FBO->bind(true);
	glActiveTexture(GL_TEXTURE0 + this->depth_cube_FBO->get_texture());
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->depth_cube_FBO->get_texture());

	// RENDER IN GAME SCENE
	this->render_level();
	this->render_blur();

	// RENDER FOREGROUND
	glClear(GL_DEPTH_BUFFER_BIT);
	ModelRenderQueue::get("foreground_animated")->render(this->dt);

	// RENDER SCREEN
	Framebuffer::bind_default(true);
	this->render_screen();

	// RENDER TEXT
	this->render_text();

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