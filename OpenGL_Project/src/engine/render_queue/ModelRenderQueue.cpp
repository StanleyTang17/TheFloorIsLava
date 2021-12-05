#include "ModelRenderQueue.h"

ModelRenderQueue* ModelRenderQueue::load(std::string name, Shader* single_main_shader)
{
	ModelRenderQueue* queue = new ModelRenderQueue(name, single_main_shader);
	LOADED_SET[queue->name] = queue;
	return queue;
}

ModelRenderQueue* ModelRenderQueue::load(std::string name, ShaderPipeline* pipeline)
{
	ModelRenderQueue* queue = new ModelRenderQueue(name, pipeline);
	LOADED_SET[queue->name] = queue;
	return queue;
}

ModelRenderQueue* ModelRenderQueue::get(std::string name)
{
	if (LOADED_SET.find(name) != LOADED_SET.end())
	{
		ModelRenderQueue* queue = static_cast<ModelRenderQueue*>(LOADED_SET.at(name));
		return queue;
	}
	return nullptr;
}

ModelRenderQueue::ModelRenderQueue(std::string name, Shader* single_main_shader)
	:
	RenderQueue(name, single_main_shader)
{}

ModelRenderQueue::ModelRenderQueue(std::string name, ShaderPipeline* pipeline)
	:
	RenderQueue(name, pipeline)
{}

void ModelRenderQueue::render_with_shader(Shader* shader, const float dt)
{
	shader->use();
	for (ModelInstance* instance : this->instances)
	{
		instance->update(dt);
		instance->render(this->main_shader, this->main_shader);
	}
}

void ModelRenderQueue::render_with_pipeline(ShaderPipeline* pipeline, const float dt)
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

void ModelRenderQueue::add_instance(ModelInstance* instance)
{
	this->instances.push_back(instance);
}

void ModelRenderQueue::remove_instance(ModelInstance* instance)
{
	for (std::vector<ModelInstance*>::iterator it = this->instances.begin(); it != this->instances.end(); ++it)
		if ((*it) == instance)
			this->instances.erase(it);
}