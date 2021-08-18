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

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0 + this->id);
	glBindTexture(this->gl_texture_type, this->id);
}

void Texture::unbind()
{
	glActiveTexture(0);
	glBindTexture(this->gl_texture_type, 0); // UNBIND TEXTURE
}

Texture2D::Texture2D(std::string type, std::string path, std::string directory)
	:
	Texture(type, GL_TEXTURE_2D)
{
	this->path = path;
	std::string image_file = directory + "/" + path;
	int channels;
	unsigned char* image = SOIL_load_image(image_file.c_str(), &this->width, &this->height, &channels, SOIL_LOAD_AUTO);

	glGenTextures(1, &this->id);

	if (image)
	{
		GLenum format = GL_RGBA;
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;

		glBindTexture(this->gl_texture_type, this->id);
		glTexImage2D(this->gl_texture_type, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(this->gl_texture_type);

		glTexParameteri(this->gl_texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(this->gl_texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(this->gl_texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(this->gl_texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "FAILED TO LOAD TEXTURE: " << image_file << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(this->gl_texture_type, 0); // UNBIND TEXTURE
	SOIL_free_image_data(image);
}

TextureCube::TextureCube(std::vector<std::string> paths, std::string directory)
	:
	Texture("cube", GL_TEXTURE_CUBE_MAP)
{
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

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

	glTexParameteri(this->gl_texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(this->gl_texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(this->gl_texture_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(this->gl_texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(this->gl_texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glActiveTexture(0);
	glBindTexture(this->gl_texture_type, 0); // UNBIND TEXTURE
}