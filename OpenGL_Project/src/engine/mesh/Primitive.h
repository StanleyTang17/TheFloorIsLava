#pragma once

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include"libs/OPENGL.h"

class Primitive
{
protected:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	unsigned int num_vertices;
	unsigned int num_indices;

	Primitive(unsigned int num_vertices, GLuint VAO, GLuint VBO, unsigned int num_indices, GLuint EBO);
	~Primitive();

public:
	void draw_vertices();
};

class Quad2D : public Primitive
{
public:
	Quad2D(float x, float y, float width, float height);
};

#endif