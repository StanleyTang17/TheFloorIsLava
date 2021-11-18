#include "Primitive.h"

Primitive::Primitive(unsigned int num_vertices, GLuint VAO, GLuint VBO, unsigned int num_indices, GLuint EBO)
{
	this->VAO = VAO;
	this->VBO = VBO;
	this->EBO = EBO;
	this->num_vertices = num_vertices;
	this->num_indices = num_indices;
}

Primitive::~Primitive()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	if (this->num_indices > 0)
		glDeleteBuffers(1, &this->EBO);
}

void Primitive::draw_vertices(GLenum mode)
{
	glBindVertexArray(this->VAO);

	if (this->num_indices == 0)
		glDrawArrays(mode, 0, this->num_vertices);
	else
		glDrawElements(mode, this->num_indices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

Quad2D::Quad2D(float x, float y, float w, float h)
	:
	Primitive(0, 0, 0, 0, 0)
{
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;

	float vertices[24] = {
		x,     y + h,   0.0f, 0.0f,
		x,     y,       0.0f, 1.0f,
		x + w, y,       1.0f, 1.0f,

		x,     y + h,   0.0f, 0.0f,
		x + w, y,       1.0f, 1.0f,
		x + w, y + h,   1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->num_vertices = 6;
}

Cube3D::Cube3D()
	:
	Primitive(0, 0, 0, 0, 0)
{
	float vertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		// Bottom face          
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left              
	};

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->num_vertices = 36;
}

Quad3D::Quad3D(glm::vec3 position, glm::vec3 normal, float width, float height)
	:
	Primitive(0, 0, 0, 0, 0)
{
	float x = position.x;
	float y = position.y;
	float z = position.z;
	float nx = normal.x;
	float ny = normal.y;
	float nz = normal.z;
	float w = width;
	float h = height;

	float vertices[48] = {
		x,     y + h,	z,		nx, ny, nz,		0.0f, 0.0f,
		x,     y,		z,		nx, ny, nz,		0.0f, 1.0f,
		x + w, y,		z,		nx, ny, nz,		1.0f, 1.0f,

		x,     y + h,   z,		nx, ny, nz,		0.0f, 0.0f,
		x + w, y,       z,		nx, ny, nz,		1.0f, 1.0f,
		x + w, y + h,   z,		nx, ny, nz,		1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->num_vertices = 6;
}