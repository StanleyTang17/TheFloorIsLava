#pragma once

#ifndef LAVA_MODEL_H
#define LAVA_MODEL_H

#include"engine/model/Model.h"

class LavaModel : public Model
{
private:
	Texture2D* flowmap;

public:
	LavaModel(std::string model_path, std::string flowmap_path);
	~LavaModel();
	
	void render(Shader* vertex_shader, Shader* fragment_shader) override;
};

#endif