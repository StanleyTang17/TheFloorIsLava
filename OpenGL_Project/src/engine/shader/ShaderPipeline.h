#pragma once

#ifndef SHADER_PIPELINE_H
#define SHADER_PIPELINE_H

#include"engine/shader/Shader.h"
#include<unordered_map>

class ShaderPipeline
{
private:
	GLuint id;
	std::string name;
	std::unordered_map<GLbitfield, Shader*> staged_shaders;

	static GLuint CURRENT_PIPELINE;
	static std::unordered_map<std::string, ShaderPipeline*> LOADED_SET;

public:
	ShaderPipeline(std::string name, std::size_t num_programs, GLbitfield stages[], Shader* programs[]);
	~ShaderPipeline();

	Shader* get_staged_shader(GLbitfield stage);
	inline std::string get_name() const { return this->name; }
	void use();
	static void unuse();

	static ShaderPipeline* load(std::string name, std::size_t num_programs, GLbitfield stages[], Shader* programs[]);
	static ShaderPipeline* get(std::string name);
	static bool remove(std::string name);
};

#endif