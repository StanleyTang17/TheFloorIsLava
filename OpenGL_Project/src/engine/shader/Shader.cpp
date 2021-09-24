#include"Shader.h"

GLuint Shader::CURRENT_SHADER = 0;

Shader::Shader(GLenum type, std::string src_path, bool seperable, int major, int minor)
{
	this->major_version = major;
	this->minor_version = minor;
	this->seperable = seperable;
	
	GLuint shader_id = this->load_shader(type, src_path.c_str());
	if (shader_id == 0)
		std::cout << "SHADER SOURCE FILE NOT FOUND AT " << src_path << std::endl;

	char info_log[512];
	GLint success;

	this->id = glCreateProgram();
	glProgramParameteri(this->id, GL_PROGRAM_SEPARABLE, seperable);

	glAttachShader(this->id, shader_id);

	glLinkProgram(this->id);

	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->id, 512, NULL, info_log);
		std::cout << "FAILED TO LINK PROGRAM!" << std::endl;
		std::cout << info_log << std::endl;
	}

	glDeleteShader(shader_id);

	glUseProgram(0);
}

Shader::Shader(std::size_t num_shaders, GLenum types[], std::string src_paths[], bool seperable, int major, int minor)
{
	this->major_version = major;
	this->minor_version = minor;
	this->seperable = seperable;

	GLuint* shader_ids = new GLuint[num_shaders];

	for (std::size_t i = 0; i < num_shaders; ++i)
	{
		shader_ids[i] = this->load_shader(types[i], src_paths[i].c_str());
		if (shader_ids[i] == 0)
			std::cout << "SHADER SOURCE FILE NOT FOUND AT " << src_paths[i] << std::endl;
	}

	char info_log[512];
	GLint success;

	this->id = glCreateProgram();
	glProgramParameteri(this->id, GL_PROGRAM_SEPARABLE, seperable);

	for (std::size_t i = 0; i < num_shaders; ++i)
		glAttachShader(this->id, shader_ids[i]);

	glLinkProgram(this->id);

	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->id, 512, NULL, info_log);
		std::cout << "FAILED TO LINK PROGRAM!" << std::endl;
		std::cout << info_log << std::endl;
	}

	for (std::size_t i = 0; i < num_shaders; ++i)
		glDeleteShader(shader_ids[i]);

	glUseProgram(0);

	delete[] shader_ids;
	shader_ids = nullptr;
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
	if (!this->in_use())
	{
		glUseProgram(this->id);
		CURRENT_SHADER = this->id;
	}
}

void Shader::unuse()
{
	if (CURRENT_SHADER != 0)
	{
		glUseProgram(0);
		CURRENT_SHADER = 0;
	}
}

void Shader::set_1i(GLint value, const GLchar* name)
{
	this->use();
	glUniform1i(glGetUniformLocation(this->id, name), value);
}

void Shader::set_1f(GLfloat value, const GLchar* name)
{
	this->use();
	glUniform1f(glGetUniformLocation(this->id, name), value);
}

void Shader::set_vec_2f(glm::vec2 value, const GLchar* name)
{
	this->use();
	glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
}

void Shader::set_vec_3f(glm::vec3 value, const GLchar* name)
{
	this->use();
	glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
}

void Shader::set_vec_4f(glm::vec4 value, const GLchar* name)
{
	this->use();
	glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
}

void Shader::set_mat_3fv(glm::mat3 value, const GLchar* name, GLboolean transpose)
{
	this->use();
	glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
}

void Shader::set_mat_4fv(glm::mat4 value, const GLchar* name, GLboolean transpose)
{
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
}