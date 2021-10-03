#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include"Primitive.h"
#include"engine/texture/Texture.h"
#include"engine/shader/Shader.h"

class Image
{
private:
	Texture2D texture;
	Quad2D quad;
	float x;
	float y;
	float width;
	float height;

public:
	Image(std::string src, float x, float y);
	Image(std::string src, float x, float y, float width, float height);

	void render(Shader* vertex_shader, Shader* fragment_shader);
	inline float get_x() const { return this->x; }
	inline float get_y() const { return this->y; }
	inline float get_width() const { return this->width; }
	inline float get_height() const { return this->height; }
	inline void set_x(float x) { this->x = x; }
	inline void set_y(float y) { this->y = y; }
};

#endif
