#include "LavaModel.h"

LavaModel::LavaModel(std::string model_path, std::string flowmap_path)
	:
	Model(model_path)
{
	this->flowmap = new Texture2D("texture_flowmap", flowmap_path);
}

LavaModel::~LavaModel()
{
	delete this->flowmap;
}

void LavaModel::render(Shader* vertex_shader, Shader* fragment_shader)
{
	//std::cout << "rendering lava" << std::endl;
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->flowmap->get_id());

	fragment_shader->set_1i(1, "flowmap_texture");
	fragment_shader->set_1f(glfwGetTime(), "time");

	for (Mesh* mesh : this->meshes)
		mesh->rendor(vertex_shader, fragment_shader);
}