#pragma once

#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include"engine/model/ModelInstance.h"
#include"engine/shader/ShaderPipeline.h"

class RenderQueue
{
private:
	std::string name;
	std::vector<ModelInstance*> instances;
	Shader* main_shader;
	ShaderPipeline* pipeline;

	static std::unordered_map<std::string, RenderQueue*> LOADED_SET;

public:
	RenderQueue(std::string name, Shader* single_main_shader);
	RenderQueue(std::string name, ShaderPipeline* pipeline);
	~RenderQueue();

	void render(const float dt);
	void add_instance(ModelInstance* instance);
	void remove_instance(ModelInstance* instance);
	inline void set_main_shader(Shader* shader) { this->main_shader = shader; }
	inline void set_pipeline(ShaderPipeline* pipeline) { this->pipeline = pipeline; }

	static RenderQueue* load(std::string name, Shader* single_main_shader);
	static RenderQueue* load(std::string name, ShaderPipeline* pipeline);
	static RenderQueue* get(std::string);
	static bool remove(std::string);
};

#endif
