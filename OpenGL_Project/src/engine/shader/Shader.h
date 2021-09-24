#pragma once

#ifndef SHADER_H
#define SHADER_H

#include"libs/NATIVE.h"
#include"libs/OPENGL.h"
#include"libs/GLM.h"
#include<fstream>

class Shader
{
private:
	GLuint id;
	int major_version;
	int minor_version;
	bool seperable;

	std::string load_shader_src(const char* file_name);
	GLuint load_shader(GLenum type, const char* file_name);
	void link_program(GLuint vertex_shader, GLuint fragment_shader, GLuint geometry_shader);

	static GLuint CURRENT_SHADER;

public:
	Shader(GLenum type, std::string src_path, bool seperable = true, int major = 4, int minor = 4);
	Shader(std::size_t num_shaders, GLenum types[], std::string src_paths[], bool seperable = false, int major = 4, int minor = 4);
	~Shader();

	void use();
	static void unuse();

	void set_1i(GLint value, const GLchar* name);
	void set_1f(GLfloat value, const GLchar* name);
	void set_vec_2f(glm::vec2 value, const GLchar* name);
	void set_vec_3f(glm::vec3 value, const GLchar* name);
	void set_vec_4f(glm::vec4 value, const GLchar* name);
	void set_mat_3fv(glm::mat3 value, const GLchar* name, GLboolean transpose);
	void set_mat_4fv(glm::mat4 value, const GLchar* name, GLboolean transpose);

	inline GLuint get_id() const { return this->id; }
	inline bool is_seperable() const { return this->seperable; }
	inline bool in_use() const { return this->id == CURRENT_SHADER; }
};

#endif