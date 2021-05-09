#pragma once

#include"libs.h"

class Game
{
private:
	// WINDOW
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int frame_buffer_width;
	int frame_buffer_height;

	// DELTA TIME
	float dt;
	float cur_time;
	float last_time;

	// KEYBOARD INPUT
	int forward_movement;
	int side_movement;
	int vertical_movement;

	// MOUSE INPUT
	double last_mouse_x;
	double last_mouse_y;
	double mouse_x;
	double mouse_y;
	double mouse_offset_x;
	double mouse_offset_y;
	bool first_mouse;

	// GL OPTIONS
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	// CAMERA
	float FOV;
	float near_plane;
	float far_plane;
	int move_dir;
	glm::mat4 projection_matrix;

	Camera* camera;

	// VECTORS
	std::vector<Shader*> shaders;
	std::vector<glm::vec3*> light_positions;
	std::vector<Model*> models;
	std::vector<Model*> transparent_models;
	std::vector<DirLight*> dir_lights;
	std::vector<PointLight*> point_lights;
	std::vector<SpotLight*> spot_lights;

	TextureCube* skybox_texture;
	GLuint skybox_VAO;

	GLuint multisample_FBO;
	GLuint multisample_RBO;
	GLuint multisample_texture;

	GLuint screen_FBO;
	GLuint screen_texture;
	GLuint screen_VAO;

	GLuint uniform_buffer;

	// INITIALIZE
	void init_GLFW();
	void init_window(const char* title, GLboolean resizable);
	void init_GLEW();
	void init_OpenGL_options();
	void init_framebuffers();
	void init_others();
	void init_matrices();
	void init_shaders();
	void init_models();
	void init_lights();
	void init_uniforms();

	// UPDATE
	void update_uniforms();
	void update_dt();
	void update_mouse_input();
	void update_keyboard_input();

public:
	// CONSTRUCTOR / DESTRUCTOR
	Game(const char* title, const int width, const int height, const int GL_major_version, const int GL_minor_version, GLboolean resizable);
	virtual ~Game();

	// GETTERS
	int get_window_should_close();

	// SETTERS
	void set_window_should_close(bool should_close);

	// REGULAR FUNCTIONS
	void update();
	void render();

	// STATIC FUNCTIONS
	static void framebuffer_resize_callback(GLFWwindow* window, int frame_buffer_width, int frame_buffer_height);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

};