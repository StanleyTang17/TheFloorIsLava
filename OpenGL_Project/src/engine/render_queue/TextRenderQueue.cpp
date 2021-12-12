#include "TextRenderQueue.h"

TextRenderQueue* TextRenderQueue::load(std::string name, Shader* single_main_shader)
{
	TextRenderQueue* queue = new TextRenderQueue(name, single_main_shader);
	LOADED_SET[queue->name] = queue;
	return queue;
}

TextRenderQueue* TextRenderQueue::load(std::string name, ShaderPipeline* pipeline)
{
	TextRenderQueue* queue = new TextRenderQueue(name, pipeline);
	LOADED_SET[queue->name] = queue;
	return queue;
}

TextRenderQueue* TextRenderQueue::get(std::string name)
{
	if (LOADED_SET.find(name) != LOADED_SET.end())
	{
		TextRenderQueue* queue = static_cast<TextRenderQueue*>(LOADED_SET.at(name));
		return queue;
	}
	return nullptr;
}

TextRenderQueue::TextRenderQueue(std::string name, Shader* single_main_shader)
	:
	RenderQueue(name, single_main_shader)
{}

TextRenderQueue::TextRenderQueue(std::string name, ShaderPipeline* pipeline)
	:
	RenderQueue(name, pipeline)
{}

void TextRenderQueue::render_with_shader(Shader* shader, const float dt)
{
	shader->use();
	for (TextInfo* text_info : this->text_queue)
	{
		if (text_info->enabled)
		{
			Font* font = Font::get(text_info->font);
			font->render_string(shader, shader, text_info->text, text_info->x, text_info->y, text_info->scale);
		}
	}
}

void TextRenderQueue::render_with_pipeline(ShaderPipeline* pipeline, const float dt)
{
	Shader::unuse();
	pipeline->use();
	Shader* vertex = this->pipeline->get_staged_shader(GL_VERTEX_SHADER_BIT);
	Shader* fragment = this->pipeline->get_staged_shader(GL_FRAGMENT_SHADER_BIT);
	for (TextInfo* text_info : this->text_queue)
	{
		if (text_info->enabled)
		{
			Font* font = Font::get(text_info->font);
			font->render_string(vertex, fragment, text_info->text, text_info->x, text_info->y, text_info->scale, text_info->color);
		}
	}
}

void TextRenderQueue::add_text(TextInfo* text_info)
{
	this->text_queue.push_back(text_info);
}

void TextRenderQueue::remove_text(TextInfo* text_info)
{
	for (std::vector<TextInfo*>::iterator it = this->text_queue.begin(); it != this->text_queue.end(); ++it)
		if ((*it) == text_info)
			this->text_queue.erase(it);
}

void TextRenderQueue::center_text_x(TextInfo& text_info)
{
	Font* font = Font::get(text_info.font);
	float center_x = font->get_center_x(text_info.text, text_info.scale, 0.0f, 1270.0f);
	text_info.x = center_x;
}