#include"Font.h"

Font::Font(std::string font_name, unsigned int size)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "COULD NOT INITIALIZE FREETYPE LIBRARY" << std::endl;
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, ("res/fonts/" + font_name + ".ttf").c_str(), 0, &face))
	{
		std::cout << "COULD NOT INITIALIZE FONT" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "FAILED TO LOAD CHARACTER " << c << std::endl;
			continue;
		}

		GLenum params[] = { GL_TEXTURE_WRAP_S,	GL_TEXTURE_WRAP_T,	GL_TEXTURE_MAG_FILTER,	GL_TEXTURE_MIN_FILTER };
		GLint values[] = {	GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_LINEAR,				GL_LINEAR };
		Texture2D* tex = new Texture2D("font", GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer, 4, params, values);
		tex->set_unit(GL_TEXTURE0);

		Character character = {
			tex,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		characters.emplace(c, character);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}