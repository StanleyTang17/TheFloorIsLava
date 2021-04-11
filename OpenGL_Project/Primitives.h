#pragma once

// NATIVE C++ LIBRARIES
#include"NATIVE.h"

// GL
#include"OPENGL.h"

// LOCAL
#include"Vertex.h"

class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

public:
	Primitive();
	virtual ~Primitive();

	void set(const Vertex* vertices, const unsigned int num_vertices, const GLuint* indices, const unsigned int num_indices);

	inline Vertex* get_vertices() { return this->vertices.data(); }
	inline GLuint* get_indices() { return this->indices.data(); }
	inline const unsigned int get_num_vertices() { return (unsigned int)this->vertices.size(); }
	inline const unsigned int get_num_indices() { return (unsigned int)this->indices.size(); }
};

class Triangle : public Primitive
{
public:
	Triangle();
};

class Quad : public Primitive
{
public:
	Quad();
};

class Pyramid : public Primitive
{
public:
	Pyramid();
};

class Cube : public Primitive
{
public:
	Cube();
};