#pragma once

#ifndef WAREHOUSE_MODEL_H
#define WAREHOUSE_MODEL_H

#include"engine/model/Model.h"

class WarehouseModel : public Model
{
protected:
	const glm::vec3 size;
	void post_load_vertex_process(std::vector<Vertex>& vertices) override;
public:
	WarehouseModel(glm::vec3 scale);
};

#endif