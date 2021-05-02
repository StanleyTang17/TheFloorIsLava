#pragma once

// NATIVE C++ LIBRARIES
#include"NATIVE.h"

// GL
#include"OPENGL.h"

// SOIL2
#include"SOIL2.h"

class Texture
{
protected:
	GLuint id;
	std::string type;
	GLenum gl_texture_type;

public:
	Texture(std::string type, GLenum gl_texture_type);
	~Texture();
	void bind();
	void unbind();
	GLuint get_id() const { return this->id; }
	std::string get_type() const { return this->type; }
};

class Texture2D : public Texture
{
private:
	int width;
	int height;
	std::string path;
public:
	Texture2D(std::string type, std::string path, std::string directory);
	std::string get_path() const { return this->path; }
};

class TextureCube : public Texture
{
public:
	TextureCube(std::vector<std::string> paths, std::string directory);
};