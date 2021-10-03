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

public:
	Font(std::string font_name, unsigned int size, glm::vec3 color);
	void render_string(Shader* vertex_shader, Shader* fragment_shader, std::string str, float x, float y, float scale);
};

#endif