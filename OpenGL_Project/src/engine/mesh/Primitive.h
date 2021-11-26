#pragma once

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include"libs/OPENGL.h"
#include"utility/Utility.h"
#include"libs/NATIVE.h"

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
	void draw_vertices(GLenum mode = GL_TRIANGLES);
	inline GLuint get_VAO() const { return this->VAO; }
	inline GLuint get_VBO() const { return this->VBO; }
	inline unsigned int get_num_vertices() const { return this->num_vertices; }
	void init_instanced_mat4(GLuint position, GLuint instanced_VBO, GLsizeiptr data_size = 0, glm::mat4* data = nullptr, GLenum usage = GL_STATIC_DRAW);
	void init_instanced_float(GLuint position, GLuint instanced_VBO, GLsizeiptr data_size = 0, float* data = nullptr, GLenum usage = GL_STATIC_DRAW);
};

class Quad2D : public Primitive
{
private:
	float x;
	float y;
	float width;
	float height;
public:
	Quad2D(float x, float y, float width, float height);

	inline float get_x() const { return this->x; }
	inline float get_y() const { return this->y; }
	inline float get_width() const { return this->width; }
	inline float get_height() const { return this->height; }
};

class Cube3D : public Primitive
{
public:
	Cube3D();
};

class Quad3D : public Primitive
{
public:
	Quad3D(glm::vec3 position, glm::vec3 normal, float width, float height);
};

#endif