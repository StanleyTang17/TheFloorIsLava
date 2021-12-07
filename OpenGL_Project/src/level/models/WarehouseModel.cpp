#include "WarehouseModel.h"

WarehouseModel::WarehouseModel(glm::vec3 scale)
	:
	size(scale)
{
	this->init("res/models/warehouse/warehouse_walls.obj");
}

void WarehouseModel::post_load_vertex_process(std::vector<Vertex>& vertices)
{
	for (std::size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position = vertices[i].position * this->size;
		vertices[i].texcoord = vertices[i].texcoord * glm::vec2(this->size) * 0.5f;
	}
}