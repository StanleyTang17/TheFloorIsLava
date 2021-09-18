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

	std::string load_shader_src(const char* file_name);
	GLuint load_shader(GLenum type, const char* file_name);
	void link_program(GLuint vertex_shader, GLuint fragment_shader, GLuint geometry_shader);

public:
	Shader(std::string directory, bool has_geometry_shader, int major_version, int minor_version);
	~Shader();

	void use();
	void unuse();

	void set_1i(GLint value, const GLchar* name);
	void set_1f(GLfloat value, const GLchar* name);
	void set_vec_2f(glm::vec2 value, const GLchar* name);
	void set_vec_3f(glm::vec3 value, const GLchar* name);
	void set_vec_4f(glm::vec4 value, const GLchar* name);
	void set_mat_3fv(glm::mat3 value, const GLchar* name, GLboolean transpose);
	void set_mat_4fv(glm::mat4 value, const GLchar* name, GLboolean transpose);

	GLuint get_id() const { return this->id; }
};

#endif