#include "Image.h"

Image::Image(std::string src, float x, float y)
	:
	texture("texture_diffuse", src), quad(0, 0, 1, 1)
{
	texture.set_unit(GL_TEXTURE0);
	this->x = x;
	this->y = y;
	this->width = texture.get_width();
	this->height = texture.get_height();
}

Image::Image(std::string src, float x, float y, float width, float height)
	:
	texture("texture_diffuse", src), quad(0, 0, 1, 1)
{
	texture.set_unit(GL_TEXTURE0);
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void Image::render(Shader* vertex_shader, Shader* fragment_shader)
{
	glm::mat4 model_matrix = Utility::generate_transform(glm::vec3(this->x, this->y, 0.0f), glm::vec3(0.0f), glm::vec3(this->width, this->height, 1.0f));
	vertex_shader->set_mat_4fv(model_matrix, "model", GL_FALSE);
	fragment_shader->set_1i(0, "image_texture");
	this->texture.bind();
	this->quad.draw_vertices();
}