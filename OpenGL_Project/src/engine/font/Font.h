#pragma once

#include<ft2build.h>
#include FT_FREETYPE_H
#include"engine/texture/Texture.h"
#include"libs/GLM.h"

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
	std::map<char, Character> characters;
public:
	Font(std::string font_name, unsigned int size);
	Character get_character(char c) const { return characters.at(c); };
};