#pragma once

// NATIVE C++ LIBRARIES
#include"NATIVE.h"

// GL
#include"OPENGL.h"

// SOIL2
#include<SOIL2.h>


class Texture
{
private:
	GLuint id;
	GLenum type;
	int width;
	int height;

public:
	Texture(const char* image_file, GLenum type);
	~Texture();
	void bind(GLint texture_unit);
	void unbind();
	inline GLuint get_id() const { return this->id; }
};