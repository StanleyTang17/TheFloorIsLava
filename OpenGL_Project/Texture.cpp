#include"Texture.h"

Texture::Texture(const char* image_file, GLenum type)
{
	this->id = 0;
	this->width = 0;
	this->height = 0;
	this->type = type;

	unsigned char* image = SOIL_load_image(image_file, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

	glGenTextures(1, &this->id);
	glBindTexture(this->type, this->id);

	glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(this->type);
	}
	else
	{
		std::cout << "FAILED TO LOAD TEXTURE: " << image_file << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(this->type, 0); // UNBIND TEXTURE
	SOIL_free_image_data(image);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->id);
}

void Texture::bind(GLint texture_unit)
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(this->type, this->id);
}

void Texture::unbind()
{
	glActiveTexture(0);
	glBindTexture(this->type, 0); // UNBIND TEXTURE
}