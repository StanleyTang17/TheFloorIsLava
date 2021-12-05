#pragma once

#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include"engine/shader/ShaderPipeline.h"

class RenderQueue
{
protected:
	std::string name;
	Shader* main_shader;
	ShaderPipeline* pipeline;

	virtual void render_with_shader(Shader* shader, const float dt) = 0;
	virtual void render_with_pipeline(ShaderPipeline* pipeline, const float dt) = 0;

	static std::unordered_map<std::string, RenderQueue*> LOADED_SET;

public:
	RenderQueue(std::string name, Shader* single_main_shader);
	RenderQueue(std::string name, ShaderPipeline* pipeline);
	~RenderQueue();

	void render(const float dt);

	inline void set_main_shader(Shader* shader) { this->main_shader = shader; }
	inline void set_pipeline(ShaderPipeline* pipeline) { this->pipeline = pipeline; }
	inline std::string get_name() const { return this->name; }

	static void add(std::string, RenderQueue* queue);
	static RenderQueue* get(std::string);
	static bool remove(std::string);
};

#endif
