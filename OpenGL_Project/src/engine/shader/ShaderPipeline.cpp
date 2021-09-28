#include "ShaderPipeline.h"

GLuint ShaderPipeline::CURRENT_PIPELINE = 0;
std::unordered_map<std::string, ShaderPipeline*> ShaderPipeline::LOADED_SET = std::unordered_map<std::string, ShaderPipeline*>();

ShaderPipeline* ShaderPipeline::load(std::string name, std::size_t num_programs, GLbitfield stages[], Shader* programs[])
{
	ShaderPipeline* pipeline = new ShaderPipeline(name, num_programs, stages, programs);
	LOADED_SET[pipeline->name] = pipeline;
	return pipeline;
}

ShaderPipeline* ShaderPipeline::get(std::string name)
{
	if (LOADED_SET.find(name) != LOADED_SET.end())
		return LOADED_SET.at(name);
	return nullptr;
}

bool ShaderPipeline::remove(std::string name)
{
	if (LOADED_SET.find(name) != LOADED_SET.end())
		return LOADED_SET.erase(name);
	return false;
}

ShaderPipeline::ShaderPipeline(std::string name, std::size_t num_programs, GLbitfield stages[], Shader* programs[])
{
	this->name = name;
	glGenProgramPipelines(1, &this->id);
	
	for (std::size_t i = 0; i < num_programs; ++i)
	{
		glUseProgramStages(this->id, stages[i], programs[i]->get_id());
	}
	
	GLint log_length;
	glGetProgramPipelineiv(this->id, GL_INFO_LOG_LENGTH, &log_length);

	if (log_length > 0)
	{
		char info_log[512];
		glGetProgramPipelineInfoLog(this->id, 512, NULL, info_log);
		std::cout << "[" << this->name << "] PIPELINE INFO LOG:" << std::endl;
		std::cout << info_log << std::endl;
	}
}

ShaderPipeline::~ShaderPipeline()
{
	glDeleteProgramPipelines(1, &this->id);
}

void ShaderPipeline::use()
{
	if (CURRENT_PIPELINE != this->id)
	{
		glBindProgramPipeline(this->id);
		CURRENT_PIPELINE = this->id;
	}
}

void ShaderPipeline::unuse()
{
	if (CURRENT_PIPELINE != 0)
	{
		glBindProgramPipeline(0);
		CURRENT_PIPELINE = 0;
	}
}