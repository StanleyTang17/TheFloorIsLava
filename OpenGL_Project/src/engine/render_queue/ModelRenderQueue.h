#pragma once

#ifndef MODEL_RENDER_QUEUE_H
#define MODEL_RENDER_QUEUE_H

#include"RenderQueue.h"
#include"engine/model/ModelInstance.h"

class ModelRenderQueue : public RenderQueue
{
private:
	std::vector<ModelInstance*> instances;

protected:
	void render_with_shader(Shader* shader, const float dt) override;
	void render_with_pipeline(ShaderPipeline* pipeline, const float dt) override;

public:
	ModelRenderQueue(std::string name, Shader* single_main_shader);
	ModelRenderQueue(std::string name, ShaderPipeline* pipeline);

	static ModelRenderQueue* load(std::string name, Shader* single_main_shader);
	static ModelRenderQueue* load(std::string name, ShaderPipeline* pipeline);
	static ModelRenderQueue* get(std::string);

	void add_instance(ModelInstance* instance);
	void remove_instance(ModelInstance* instance);
};

#endif