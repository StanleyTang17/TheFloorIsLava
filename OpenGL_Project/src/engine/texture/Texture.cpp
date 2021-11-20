#include"Texture.h"

Texture::Texture(std::string type, GLenum gl_texture_type)
{
	this->id = 0;
	this->type = type;
	this->gl_texture_type = gl_texture_type;
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->id);
}

void Texture::generate()
{
	glGenTextures(1, &this->id);
	if (id < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
		this->unit = GL_TEXTURE0 + id;
	else
		this->unit = GL_TEXTURE0 + GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1;
}

void Texture::bind()
{
	glActiveTexture(this->unit);
	glBindTexture(this->gl_texture_type, this->id);
}

void Texture::unbind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(this->gl_texture_type, 0); // UNBIND TEXTURE
}

void Texture::tex_parameteri(GLenum param_name, GLint param_value)
{
	this->bind();
	glTexParameteri(this->gl_texture_type, param_name, param_value);
	this->unbind();
}

void Texture::tex_parameteri(std::size_t num_params, GLenum param_names[], GLint param_values[])
{
	this->bind();
	for (std::size_t i = 0; i < num_params; ++i)
	{
		glTexParameteri(this->gl_texture_type, param_names[i], param_values[i]);
	}
	this->unbind();
}

Texture2D::Texture2D(std::string type, std::string file_path)
	:
	Texture(type, GL_TEXTURE_2D)
{
	this->path = path;
	int channels;
	unsigned char* image = SOIL_load_image(file_path.c_str(), &this->width, &this->height, &channels, SOIL_LOAD_AUTO);

	this->generate();

	if (image)
	{
		GLenum format = GL_RGBA;
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;

		this->bind();
		glTexImage2D(this->gl_texture_type, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(this->gl_texture_type);

		GLenum params[] = { GL_TEXTURE_WRAP_S,	GL_TEXTURE_WRAP_T,	GL_TEXTURE_MAG_FILTER,		GL_TEXTURE_MIN_FILTER };
//		GLint values[] = {	GL_REPEAT,			GL_REPEAT,			GL_LINEAR_MIPMAP_LINEAR,	GL_LINEAR };
		GLint values[] = { GL_REPEAT,			GL_REPEAT,			GL_NEAREST,					GL_NEAREST };
		this->tex_parameteri(4, params, values);
	}
	else
	{
		std::cout << "FAILED TO LOAD TEXTURE: " << file_path << std::endl;
	}

	this->unbind();
	SOIL_free_image_data(image);
}

Texture2D::Texture2D(std::string type, GLenum format, int width, int height, const void* pixels, std::size_t num_params, GLenum params[], GLint values[])
	:
	Texture(type, GL_TEXTURE_2D)
{
	this->path = "";
	this->generate();
	this->bind();

	glTexImage2D(this->gl_texture_type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(this->gl_texture_type);
	this->tex_parameteri(num_params, params, values);

	this->unbind();
}

TextureCube::TextureCube(std::vector<std::string> paths, std::string directory)
	:
	Texture("cube", GL_TEXTURE_CUBE_MAP)
{
	this->generate();
	this->bind();

	if (paths.size() >= 6)
	{
		for (std::size_t i = 0; i < 6; ++i)
		{
			std::string image_file = directory + "/" + paths[i];
			int channels, width, height;
			unsigned char* image = SOIL_load_image(image_file.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);

			if (image)
			{
				GLenum format = GL_RGBA;
				if (channels == 1)
					format = GL_RED;
				else if (channels == 3)
					format = GL_RGB;
				else if (channels == 4)
					format = GL_RGBA;

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
				SOIL_free_image_data(image);
			}
			else
			{
				std::cout << "FAILED TO LOAD TEXTURE: " << image_file << std::endl;
				break;
			}
		}
	}
	else
	{
		std::cout << "NOT ENOUGH TEXTURES FOR CUBEMAP" << std::endl;
	}

	GLenum params[] = { GL_TEXTURE_WRAP_S,	GL_TEXTURE_WRAP_T,	GL_TEXTURE_WRAP_R,	GL_TEXTURE_MAG_FILTER,	GL_TEXTURE_MIN_FILTER };
	GLint values[] = {	GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_LINEAR,				GL_LINEAR };
	this->tex_parameteri(5, params, values);

	this->unbind();
}

TextureAtlas2D::TextureAtlas2D(std::string type, std::string path, int rows, int cols, int num_sprites)
	:
	Texture2D(type, path)
{
	this->rows = rows;
	this->cols = cols;
	if (num_sprites == -1)
		this->num_sprites = rows * cols;
	else
		this->num_sprites = num_sprites;
}