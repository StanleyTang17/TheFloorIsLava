#pragma once

// NATIVE C++ LIBRARIES
#include"NATIVE.h"

// GL
#include"OPENGL.h"

// SOIL2
#include"SOIL2.h"

class Texture
{
private:
	GLuint id;
	std::string type;
	std::string path;
	GLenum gl_texture_type;
	int width;
	int height;

public:
	Texture(std::string type, std::string path, std::string directory, GLenum gl_texture_type);
	~Texture();
	void bind();
	void unbind();
	GLuint get_id() const { return this->id; }
	std::string get_type() const { return this->type; }
	std::string get_path() const { return this->path; }
};