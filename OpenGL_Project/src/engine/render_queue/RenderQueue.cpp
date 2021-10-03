#include "RenderQueue.h"

std::unordered_map<std::string, RenderQueue*> RenderQueue::LOADED_SET = std::unordered_map<std::string, RenderQueue*>();

RenderQueue* RenderQueue::load(std::string name, Shader* single_main_shader)
{
	RenderQueue* queue = new RenderQueue(name, single_main_shader);
	LOADED_SET[queue->name] = queue;
	return queue;
}

RenderQueue* RenderQueue::load(std::string name, ShaderPipeline* pipeline)
{
	RenderQueue* queue = new RenderQueue(name, pipeline);
	LOADED_SET[queue->name] = queue;
	return queue;
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
	{
		this->main_shader->use();
		for (ModelInstance* instance : this->instances)
		{
			instance->update(dt);
			instance->render(this->main_shader, this->main_shader);
		}
	}
	else if (this->pipeline)
	{
		Shader::unuse();
		this->pipeline->use();
		Shader* vertex = this->pipeline->get_staged_shader(GL_VERTEX_SHADER_BIT);
		Shader* fragment = this->pipeline->get_staged_shader(GL_FRAGMENT_SHADER_BIT);
		for (ModelInstance* instance : this->instances)
		{
			instance->update(dt);
			instance->render(vertex, fragment);
		}
	}
}

void RenderQueue::add_instance(ModelInstance* instance)
{
	this->instances.push_back(instance);
}

void RenderQueue::remove_instance(ModelInstance* instance)
{
	for (std::vector<ModelInstance*>::iterator it = this->instances.begin(); it != this->instances.end(); ++it)
		if ((*it) == instance)
			this->instances.erase(it);
}

RenderQueue::~RenderQueue()
{
	RenderQueue::remove(this->name);
}