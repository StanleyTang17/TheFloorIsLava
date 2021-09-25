#pragma once

#ifndef SHADER_PIPELINE_H
#define SHADER_PIPELINE_H

#include"engine/shader/Shader.h"

class ShaderPipeline
{
private:
	GLuint id;

	static GLuint CURRENT_PIPELINE;

public:
	ShaderPipeline(std::size_t num_programs, GLbitfield stages[], Shader* programs[]);
	~ShaderPipeline();

	void use();
	void unuse();
};

#endif