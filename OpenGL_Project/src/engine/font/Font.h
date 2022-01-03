#pragma once

#ifndef FONT_H
#define FONT_H

#include<ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
#include"engine/texture/Texture.h"
#include"engine/mesh/Primitive.h"
#include"engine/shader/Shader.h"
#include"utility/Utility.h"

struct Character
{
	Texture2D* texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class Font
{
private:
	std::unordered_map<char, Character> characters;
	glm::vec4 text_color;
	glm::vec4 outline_color;
	Quad2D char_quad;

	static std::unordered_map<std::string, Font*> LOADED_SET;

	bool load_char(Character& character, FT_Face& face, char ch);
	bool load_outlined_char(Character& character, FT_Face& face, FT_Library &ft, char ch, float outline_thickness);

public:
	Font(std::string font_family, unsigned int size, glm::vec4 text_color, glm::vec4 outline_color = glm::vec4(1.0f), float outline_thickness = 0.0f);
	void render_string(Shader* vertex_shader, Shader* fragment_shader, std::string str, float y, float scale);
	void render_string(Shader* vertex_shader, Shader* fragment_shader, std::string str, float x, float y, float scale);
	glm::vec2 get_string_dimension(std::string str, float scale);
	float get_center_x(std::string str, float scale, float lower_x, float upper_x);

	inline glm::vec4 get_text_color() const { return this->text_color; }
	inline glm::vec4 get_outline_color() const { return this->outline_color; }
	inline void set_text_color(glm::vec4 color) { this->text_color = color; }
	inline void set_outline_color(glm::vec4 color) { this->outline_color = color; }

	static Font* load(std::string name, std::string font_family, unsigned int size, glm::vec4 color);
	static Font* load(std::string name, std::string font_family, unsigned int size, glm::vec4 text_color, glm::vec4 outline_color, float outline_thickness);
	static Font* get(std::string name);
};

#endif