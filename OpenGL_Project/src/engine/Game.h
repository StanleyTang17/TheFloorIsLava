#pragma once

#include"libs/ALL.h"
#include"GlobalDebug.h"
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
	const int FPS = 60;

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
	glm::mat4 light_space_matrix;

	Camera* camera;

	int show_depth;

	// VECTORS
	std::vector<Shader*> shaders;
	std::vector<Model*> transparent_models;
	std::vector<DirLight*> dir_lights;
	std::vector<PointLight*> point_lights;
	std::vector<SpotLight*> spot_lights;
	std::vector<GameObject*> game_objects;

	std::map<ModelClass, Model*>models;

	// SKYBOX

	TextureCube* skybox_texture;
	GLuint skybox_VAO;

	// OTHER VAOs and VBOs;

	GLuint screen_VAO;
	GLuint text_VAO;
	GLuint text_VBO;
	Texture2D* test_texture;

	GLuint uniform_buffer;

	// FRAMEBUFFERS

	MultiSampleFramebuffer* multisample_FBO;
	ScreenFramebuffer* screen_FBO;
	DepthFramebuffer* depth_FBO;
	DepthCubeFramebuffer* depth_cube_FBO;

	// GAMEOBJECTS

	Player* player;
	Crate* crate;
	Crate* crate2;
	bool hit;

	// FONTS

	Font* arial;
	Font* arial_big;

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
	void init_game_objects();
	void init_lights();
	void init_uniforms();
	void init_fonts();


	// UPDATE
	void update_uniforms();
	void update_dt();
	void update_mouse_input();
	void update_keyboard_input();

	// DRAW
	void render_skybox(Shader* shader);
	void render_models(Shader* shader);
	void render_screen();
	void render_text(Shader* shader, Font* font, std::string text, float x, float y, float scale, glm::vec3 color);

public:
	// CONSTRUCTOR / DESTRUCTOR
	Game(const char* title, const int width, const int height, const int GL_major_version, const int GL_minor_version, GLboolean resizable);
	~Game();

	// GETTERS
	int get_window_should_close();

	// SETTERS
	void set_window_should_close(bool should_close);

	// REGULAR FUNCTIONS
	void update();
	void render();
	void add_game_object(GameObject* game_object);
	void remove_game_object(GameObject* game_object);

	// STATIC FUNCTIONS
	static void framebuffer_resize_callback(GLFWwindow* window, int frame_buffer_width, int frame_buffer_height);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

};