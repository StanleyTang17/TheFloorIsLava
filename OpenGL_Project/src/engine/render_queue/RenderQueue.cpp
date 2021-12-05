#include "RenderQueue.h"

std::unordered_map<std::string, RenderQueue*> RenderQueue::LOADED_SET = std::unordered_map<std::string, RenderQueue*>();

void RenderQueue::add(std::string name, RenderQueue* queue)
{
	LOADED_SET[name] = queue;
}

RenderQueue* RenderQueue::get(std::string name)
{
	if (LOADED_SET.find(name) != LOADED_SET.end())
		return LOADED_SET.at(name);
	return nullptr;
}

bool RenderQueue::remove(std::string name)
{
	if (LOADED_SET.find(name) != LOADED_SET.end())
		return LOADED_SET.erase(name);
	return false;
}

RenderQueue::RenderQueue(std::string name, Shader* single_main_shader)
{
	this->name = name;
	this->main_shader = single_main_shader;
	this->pipeline = nullptr;
}

RenderQueue::RenderQueue(std::string name, ShaderPipeline* pipeline)
{
	this->name = name;
	this->main_shader = nullptr;
	this->pipeline = pipeline;
}

void RenderQueue::render(const float dt)
{
	if (this->main_shader)
		this->render_with_shader(this->main_shader, dt);
	else if (this->pipeline)
		this->render_with_pipeline(this->pipeline, dt);
}

RenderQueue::~RenderQueue()
{
	RenderQueue::remove(this->name);
}