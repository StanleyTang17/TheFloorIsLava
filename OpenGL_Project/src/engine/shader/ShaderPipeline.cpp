#include "ShaderPipeline.h"

GLuint ShaderPipeline::CURRENT_PIPELINE = 0;

ShaderPipeline::ShaderPipeline(std::size_t num_programs, GLbitfield stages[], Shader* programs[])
{
	glGenProgramPipelines(1, &this->id);
	
	std::cout << "setting up program stages" << std::endl;
	for (std::size_t i = 0; i < num_programs; ++i)
	{
		glUseProgramStages(this->id, stages[i], programs[i]->get_id());
		std::cout << programs[i]->get_id() << std::endl;
	}
	std::cout << std::endl;

	char info_log[512];
	glGetProgramPipelineInfoLog(this->id, 512, NULL, info_log);

	std::cout << info_log << std::endl;
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