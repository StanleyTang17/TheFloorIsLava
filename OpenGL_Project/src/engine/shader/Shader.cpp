#include"Shader.h"

Shader::Shader(std::string directory, bool has_geometry_shader, int major_version, int minor_version)
{
	GLuint vertex_shader = 0;
	GLuint fragment_shader = 0;
	GLuint geometry_shader = 0;
	this->major_version = major_version;
	this->minor_version = minor_version;

	if (directory.back() != '/')
		directory += '/';

	vertex_shader = this->load_shader(GL_VERTEX_SHADER, (directory + "vertex.glsl").c_str());
	if (vertex_shader == 0)
		std::cout << "VERTEX SHADER SOURCE FILE NOT FOUND!" << std::endl;

	fragment_shader = this->load_shader(GL_FRAGMENT_SHADER, (directory + "fragment.glsl").c_str());
	if (fragment_shader == 0)
		std::cout << "FRAGMENT SHADER SOURCE FILE NOT FOUND!" << std::endl;

	if (has_geometry_shader)
	{
		geometry_shader = this->load_shader(GL_GEOMETRY_SHADER, (directory + "geometry.glsl").c_str());
		if (geometry_shader == 0)
			std::cout << "GEOMETRY SHADER SOURCE FILE NOT FOUND!";
	}

	this->link_program(vertex_shader, fragment_shader, geometry_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(geometry_shader);
	glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
	glDeleteProgram(this->id);
}

std::string Shader::load_shader_src(const char* file_name)
{
	std::string line = "";
	std::string src = "";
	std::string version_line = "#version " + std::to_string(this->major_version) + std::to_string(this->minor_version) + "0\n\n";

	std::fstream in_file;

	in_file.open(file_name);
	if (in_file.is_open())
	{
		while (std::getline(in_file, line))
		{
			src += line + "\n";
		}
	}
	else
	{
		in_file.close();
		return "FILE NOT FOUND";
	}
	in_file.close();

	if (src.find("#version") == std::string::npos) {
		src = version_line + src;
	}

	return src;
}

GLuint Shader::load_shader(GLenum type, const char* file_name)
{
	std::string src_str = this->load_shader_src(file_name);

	if (src_str == "FILE NOT FOUND")
		return 0;

	const GLchar* src = src_str.c_str();
	char info_log[512];
	GLint success;
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		std::cout << "FAILED TO COMPILE SHADER!" << std::endl;
		std::cout << info_log << std::endl;
		std::cout << src << std::endl;
	}
	
	return shader;
}

void Shader::link_program(GLuint vertex_shader, GLuint fragment_shader, GLuint geometry_shader)
{
	char info_log[512];
	GLint success;

	this->id = glCreateProgram();

	glAttachShader(this->id, vertex_shader);
	if (geometry_shader)
	{
		glAttachShader(this->id, geometry_shader);
	}
	glAttachShader(this->id, fragment_shader);

	glLinkProgram(this->id);

	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->id, 512, NULL, info_log);
		std::cout << "FAILED TO LINK PROGRAM!" << std::endl;
		std::cout << info_log << std::endl;
	}

	glUseProgram(0);
}

void Shader::use()
{
	glUseProgram(this->id);
}

void Shader::unuse()
{
	glUseProgram(0);
}

void Shader::set_1i(GLint value, const GLchar* name)
{
	this->use();
	glUniform1i(glGetUniformLocation(this->id, name), value);
	this->unuse();
}

void Shader::set_1f(GLfloat value, const GLchar* name)
{
	this->use();
	glUniform1f(glGetUniformLocation(this->id, name), value);
	this->unuse();
}

void Shader::set_vec_2f(glm::vec2 value, const GLchar* name)
{
	this->use();
	glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
	this->unuse();
}

void Shader::set_vec_3f(glm::vec3 value, const GLchar* name)
{
	this->use();
	glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
	this->unuse();
}

void Shader::set_vec_4f(glm::vec4 value, const GLchar* name)
{
	this->use();
	glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
	this->unuse();
}

void Shader::set_mat_3fv(glm::mat3 value, const GLchar* name, GLboolean transpose)
{
	this->use();
	glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
	this->unuse();
}

void Shader::set_mat_4fv(glm::mat4 value, const GLchar* name, GLboolean transpose)
{
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
	this->unuse();
}