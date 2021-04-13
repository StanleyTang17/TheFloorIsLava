#include"Primitives.h"

Primitive::Primitive() {}

Primitive::~Primitive() {}

void Primitive::set(const Vertex* vertices, const unsigned int num_vertices, const GLuint* indices, const unsigned int num_indices)
{
	this->vertices.clear();
	this->indices.clear();
	for (std::size_t i = 0; i < num_vertices; ++i)
	{
		this->vertices.push_back(vertices[i]);
	}
	for (std::size_t i = 0; i < num_indices; ++i)
	{
		this->indices.push_back(indices[i]);
	}
}

//Triangle::Triangle()
//{
//	Vertex vertices[] =
//	{
//		// position						// color						// texcoord					// normal
//		glm::vec3(0.f, 0.5f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),		glm::vec3(0.0f, 0.0f, 1.0f),
//		glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 1.0f),		glm::vec3(0.0f, 0.0f, 1.0f),
//		glm::vec3(0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),
//		//	  0
//		//	 / \
//		//  1---2
//	};
//	unsigned int num_vertices = sizeof(vertices) / sizeof(Vertex);
//
//	GLuint indices[] =
//	{
//		0, 1, 2
//	};
//	unsigned int num_indices = sizeof(indices) / sizeof(GLuint);
//
//	this->set(vertices, num_vertices, indices, num_indices);
//}
//
//Quad::Quad()
//{
//	Vertex vertices[] =
//	{
//		// position						// color						// texcoord					// normal
//		glm::vec3(-0.5f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),		glm::vec3(0.0f, 0.0f, 1.0f),
//		glm::vec3(0.5f, 0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 1.0f),		glm::vec3(0.0f, 0.0f, 1.0f),
//		glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),
//		glm::vec3(0.5f, -0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f)
//		//	0---1
//		//	|	|
//		//  2---3
//	};
//	unsigned int num_vertices = sizeof(vertices) / sizeof(Vertex);
//
//	GLuint indices[] =
//	{
//		0, 2, 3,
//		0, 3, 1
//	};
//	unsigned int num_indices = sizeof(indices) / sizeof(GLuint);
//
//	this->set(vertices, num_vertices, indices, num_indices);
//}
//
//Pyramid::Pyramid()
//{
//	Vertex vertices[] =
//	{	
//		// position						// color						// texcoord					// normal
//
//		// front
//		glm::vec3(0.0f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.5f, 1.0f),		glm::vec3(0.0f, 0.5f, 1.0f),
//		glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 0.5f, 1.0f),
//		glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 0.5f, 1.0f),
//
//		// back
//		glm::vec3(0.0f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.5f, 1.0f),		glm::vec3(0.0f, 0.5f, -1.0f),
//		glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 0.5f, -1.0f),
//		glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 0.5f, -1.0f),
//
//		// left
//		glm::vec3(0.0f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.5f, 1.0f),		glm::vec3(-1.0f, 0.5f, 0.0f),
//		glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),		glm::vec3(-1.0f, 0.5f, 0.0f),
//		glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 0.0f),		glm::vec3(-1.0f, 0.5f, 0.0f),
//
//		// right
//		glm::vec3(0.0f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.5f, 1.0f),		glm::vec3(1.0f, 0.5f, 0.0f),
//		glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),		glm::vec3(1.0f, 0.5f, 0.0f),
//		glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 0.0f),		glm::vec3(1.0f, 0.5f, 0.0f),
//
//	};
//	unsigned int num_vertices = sizeof(vertices) / sizeof(Vertex);
//
//	this->set(vertices, num_vertices, nullptr, 0);
//}

Cube::Cube()
{
	Vertex vertices[] = {
		// positions          // normals           // texture coords
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f,  0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  0.0f),
		glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  1.0f),
		glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  1.0f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f,  1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f,  0.0f),

		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f,  0.0f),
		glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f,  0.0f),
		glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f,  1.0f),
		glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f,  1.0f),
		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f,  1.0f),
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f,  0.0f),

		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f),
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  0.0f),
		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f),

		glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f),
		glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f),
		glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  0.0f),
		glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f,  1.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f,  1.0f),
		glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f,  0.0f),
		glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f,  0.0f),
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f,  0.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f,  1.0f),

		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f,  1.0f),
		glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f,  1.0f),
		glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f,  0.0f),
		glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f,  0.0f),
		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f,  0.0f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f,  1.0f),
	};
	unsigned num_vertices = sizeof(vertices) / sizeof(Vertex);

	this->set(vertices, num_vertices, nullptr, 0);
}