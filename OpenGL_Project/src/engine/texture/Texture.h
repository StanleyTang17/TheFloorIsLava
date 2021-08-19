#pragma once

#include"libs/NATIVE.h"
#include"libs/OPENGL.h"
#include"soil2/SOIL2.h"

class Texture
{
protected:
	GLuint id;
	GLenum unit;
	std::string type;
	GLenum gl_texture_type;

	void generate();

public:
	Texture(std::string type, GLenum gl_texture_type);
	~Texture();
	void bind();
	void unbind();
	void tex_parameteri(GLenum param_name, GLint param_value);
	void tex_parameteri(std::size_t num_params, GLenum param_names[], GLint param_values[]);
	void set_unit(GLenum texture_unit) { this->unit = texture_unit; }
	GLuint get_id() const { return this->id; }
	GLenum get_unit() const { return this->unit; }
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
	Texture2D(std::string type, GLenum format, int width, int height, const void* pixels, std::size_t num_params, GLenum params[], GLint values[]);
	std::string get_path() const { return this->path; }
};

class TextureCube : public Texture
{
public:
	TextureCube(std::vector<std::string> paths, std::string directory);
};