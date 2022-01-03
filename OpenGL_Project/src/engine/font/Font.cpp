#include"Font.h"

std::unordered_map<std::string, Font*> Font::LOADED_SET = std::unordered_map<std::string, Font*>();

Font* Font::load(std::string name, std::string font_family, unsigned int size, glm::vec4 text_color)
{
	Font* font = new Font(font_family, size, text_color);
	LOADED_SET[name] = font;
	return font;
}

Font* Font::load(std::string name, std::string font_family, unsigned int size, glm::vec4 text_color, glm::vec4 outline_color, float outline_thickness)
{
	Font* font = new Font(font_family, size, text_color, outline_color, outline_thickness);
	LOADED_SET[name] = font;
	return font;
}

Font* Font::get(std::string name)
{
	if (LOADED_SET.find(name) != LOADED_SET.end())
		return LOADED_SET.at(name);
	return nullptr;
}

Font::Font(std::string font_family, unsigned int size, glm::vec4 text_color, glm::vec4 outline_color, float outline_thickness)
	:
	char_quad(0.0f, 0.0f, 1.0f, 1.0f)
{
	this->text_color = text_color;
	this->outline_color = outline_color;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "COULD NOT INITIALIZE FREETYPE LIBRARY" << std::endl;
		return;
	}
	
	FT_Face face;
	if (FT_New_Face(ft, ("res/fonts/" + font_family + ".ttf").c_str(), 0, &face))
	{
		std::cout << "COULD NOT INITIALIZE FONT" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; ++c)
	{
		Character character;

		if (outline_thickness > 0.0f)
		{
			if (this->load_outlined_char(character, face, ft, c, outline_thickness))
				characters.emplace(c, character);
		}
		else
		{
			if (this->load_char(character, face, c))
				characters.emplace(c, character);
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

bool Font::load_char(Character& character, FT_Face& face, char ch)
{
	if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
	{
		std::cout << "FAILED TO LOAD FACE FOR CHARACTER " << ch << std::endl;
		return false;
	}

	GLenum params[] = { GL_TEXTURE_WRAP_S,	GL_TEXTURE_WRAP_T,	GL_TEXTURE_MAG_FILTER,	GL_TEXTURE_MIN_FILTER };
	GLint values[] = { GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_LINEAR,				GL_LINEAR };
	Texture2D* tex = new Texture2D("font", GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer, 4, params, values);
	tex->set_unit(GL_TEXTURE0);

	character.texture = tex;
	character.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
	character.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
	character.advance = face->glyph->advance.x;

	return true;
}

bool Font::load_outlined_char(Character& character, FT_Face& face, FT_Library& ft, char ch, float outline_thickness)
{
	if (FT_Load_Char(face, ch, FT_LOAD_NO_BITMAP | FT_LOAD_TARGET_NORMAL))
	{
		std::cout << "FAILED TO LOAD FACE FOR CHARACTER " << ch << std::endl;
		return false;
	}

	// LOAD STROKE

	FT_Glyph stroke_glyph;
	if (FT_Get_Glyph(face->glyph, &stroke_glyph))
	{
		std::cout << "FAILED TO LOAD STROKE GLYPH FOR CHARACTER " << ch << std::endl;
		return false;
	}

	FT_Stroker stroker;
	if (FT_Stroker_New(ft, &stroker))
	{
		std::cout << "FAILED TO LOAD STROKER FOR CHARACTER" << ch << std::endl;
		return false;
	}

	FT_Fixed radius = static_cast<FT_Fixed>(outline_thickness * static_cast<float>(1 << 6));
	FT_Stroker_Set(stroker, radius, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

	if (FT_Glyph_Stroke(&stroke_glyph, stroker, false))
	{
		std::cout << "FAILED TO LOAD GLYPH STROKE FOR CHARACTER " << ch << std::endl;
		return false;
	}

	FT_BitmapGlyph stroke_bitmap_glyph;
	if (FT_Glyph_To_Bitmap(&stroke_glyph, FT_RENDER_MODE_NORMAL, 0, true))
	{
		std::cout << "FAILED TO CONVERT STORKE GLYPH TO BITMAP FOR CHARACTER " << ch << std::endl;
		return false;
	}
	stroke_bitmap_glyph = (FT_BitmapGlyph)stroke_glyph;
	FT_Bitmap*  stroke_bitmap = &stroke_bitmap_glyph->bitmap;

	// LOAD FILL

	FT_Glyph fill_glyph;
	if (FT_Get_Glyph(face->glyph, &fill_glyph))
	{
		std::cout << "FAILED TO LOAD FILL GLYPH FOR CHARACTER " << ch << std::endl;
		return false;
	}

	FT_BitmapGlyph fill_bitmap_glyph;
	if (FT_Glyph_To_Bitmap(&fill_glyph, FT_RENDER_MODE_NORMAL, 0, true))
	{
		std::cout << "FAILED TO CONVERT FILL GLYPH TO BITMAP FOR CHARACTER " << ch << std::endl;
		return false;
	}
	fill_bitmap_glyph = (FT_BitmapGlyph)fill_glyph;
	FT_Bitmap* fill_bitmap = &fill_bitmap_glyph->bitmap;

	// GENERATE TEXTURE

	glm::ivec2 stroke_size = glm::ivec2(stroke_bitmap->width, stroke_bitmap->rows);
	glm::ivec2 fill_size = glm::ivec2(fill_bitmap->width, fill_bitmap->rows);
	glm::ivec2 offset = (stroke_size - fill_size) / 2;

	unsigned int buffer_size = stroke_size.x * stroke_size.y * 2;
	std::vector<unsigned char> buffer(buffer_size, 0);

	for (int i = 0; i < stroke_size.x * stroke_size.y; ++i)
	{
		unsigned int target_index = i * 2 + 1;
		buffer[target_index] = stroke_bitmap->buffer[i];
	}

	for (unsigned int x = 0; x < fill_size.x; ++x)
		for (unsigned int y = 0; y < fill_size.y; ++y)
		{
			unsigned int source_index = y * fill_size.x + x;
			unsigned int target_index = ((y + offset.y) * stroke_size.x + (x + offset.x)) * 2;
			buffer[target_index] = fill_bitmap->buffer[source_index];
		}

	GLenum params[] = { GL_TEXTURE_WRAP_S,	GL_TEXTURE_WRAP_T,	GL_TEXTURE_MAG_FILTER,	GL_TEXTURE_MIN_FILTER };
	GLint values[] = { GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_LINEAR,				GL_LINEAR };
	Texture2D* tex = new Texture2D("font_texture", GL_RG, stroke_size.x, stroke_size.y, buffer.data(), 4, params, values);
	tex->set_unit(GL_TEXTURE0);

	character.texture = tex;
	character.size = stroke_size;
	character.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
	character.advance = face->glyph->advance.x;

	FT_Done_Glyph(stroke_glyph);
	FT_Done_Glyph(fill_glyph);

	return true;
}

void Font::render_string(Shader* vertex_shader, Shader* fragment_shader, std::string str, float y, float scale)
{
	float x = this->get_center_x(str, scale, 0.0f, 1270.0f);
	this->render_string(vertex_shader, fragment_shader, str, x, y, scale);
}

void Font::render_string(Shader* vertex_shader, Shader* fragment_shader, std::string str, float x, float y, float scale)
{
	fragment_shader->set_1i(0, "font_texture");
	fragment_shader->set_vec_4f(this->text_color, "text_color");
	fragment_shader->set_vec_4f(this->outline_color, "outline_color");

	for (std::string::const_iterator c = str.begin(); c != str.end(); ++c)
	{
		Character ch = this->characters.at(*c);

		float x_pos = x + ch.bearing.x * scale;
		float y_pos = y + (ch.bearing.y - ch.size.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		glm::mat4 model_matrix = Utility::generate_transform(glm::vec3(x_pos, y_pos, 0.0f), glm::vec3(0.0f), glm::vec3(w, h, 1.0f));
		vertex_shader->set_mat_4fv(model_matrix, "model", GL_FALSE);

		ch.texture->bind();
		char_quad.draw_vertices();

		x += (ch.advance >> 6) * scale;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

glm::vec2 Font::get_string_dimension(std::string str, float scale)
{
	glm::vec2 size(0.0f, 0.0f);

	for (std::string::const_iterator c = str.begin(); c != str.end(); ++c)
	{
		Character ch = this->characters.at(*c);

		float char_width = (ch.advance >> 6) * scale;
		float char_height = ch.size.y;

		size.x += char_width;
		if (char_height > size.y)
			size.y = char_height;
	}

	return size;
}

float Font::get_center_x(std::string str, float scale, float lower_x, float upper_x)
{
	glm::vec2 size = this->get_string_dimension(str, scale);
	float dif = upper_x - lower_x;
	return lower_x + (dif - size.x) / 2;
}