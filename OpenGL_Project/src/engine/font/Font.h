#pragma once

#ifndef FONT_H
#define FONT_H

#include<ft2build.h>
#include FT_FREETYPE_H
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
	glm::vec3 color;
	Quad2D char_quad;

	static std::unordered_map<std::string, Font*> LOADED_SET;

public:
	Font(std::string font_family, unsigned int size, glm::vec3 color);
	void render_string(Shader* vertex_shader, Shader* fragment_shader, std::string str, float x, float y, float scale);
	glm::vec2 get_string_dimension(std::string str, float scale);
	float get_center_x(std::string str, float scale, float lower_x, float upper_x);

	static Font* load(std::string name, std::string font_family, unsigned int size, glm::vec3 color);
	static Font* get(std::string name);
};

#endif