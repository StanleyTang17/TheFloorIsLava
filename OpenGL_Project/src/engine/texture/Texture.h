#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

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
	Texture2D(std::string type, std::string path);
	Texture2D(std::string type, GLenum format, int width, int height, const void* pixels, std::size_t num_params, GLenum params[], GLint values[]);
	inline std::string get_path() const { return this->path; }
	inline int get_width() const { return this->width; }
	inline int get_height() const { return this->height; }
};

class TextureAtlas2D : public Texture2D
{
private:
	int rows;
	int cols;
	int num_sprites;
public:
	TextureAtlas2D(std::string type, std::string path, int rows, int cols, int num_sprites = -1);
	inline int get_rows() const { return this->rows; }
	inline int get_cols() const { return this->cols; }
	inline int get_num_sprites() const { return this->num_sprites; }
};

class TextureCube : public Texture
{
public:
	TextureCube(std::vector<std::string> paths, std::string directory);
};

#endif