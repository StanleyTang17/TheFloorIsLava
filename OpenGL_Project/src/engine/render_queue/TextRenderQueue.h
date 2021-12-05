#pragma once

#ifndef TEXT_RENDER_QUEUE_H
#define TEXT_RENDER_QUEUE_H

#include"RenderQueue.h"
#include"engine/font/Font.h"

struct TextInfo
{
	std::string text;
	std::string font;
	float x;
	float y;
	float scale;
	bool enabled;
};

class TextRenderQueue : public RenderQueue
{
private:
	std::vector<TextInfo*> text_queue;

protected:
	void render_with_shader(Shader* shader, const float dt) override;
	void render_with_pipeline(ShaderPipeline* pipeline, const float dt) override;

public:
	TextRenderQueue(std::string name, Shader* single_main_shader);
	TextRenderQueue(std::string name, ShaderPipeline* pipeline);

	static TextRenderQueue* load(std::string name, Shader* single_main_shader);
	static TextRenderQueue* load(std::string name, ShaderPipeline* pipeline);
	static TextRenderQueue* get(std::string);

	void add_text(TextInfo* text_info);
	void remove_text(TextInfo* text_info);
};

#endif