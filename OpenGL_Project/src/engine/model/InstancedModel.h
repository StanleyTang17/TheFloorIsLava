#pragma once

#ifndef INSTANCED_MODEL_H
#define INSTANCED_MODEL_H

#include"engine/model/ModelInstance.h"
#include"libs/NATIVE.h"

class InstancedModel : public Model
{
protected:
	std::vector<ModelInstance*> instances;
	GLuint instance_buffer;
	int last_rendered_instances;

public:
	InstancedModel(std::string model_path);
	~InstancedModel();

	void add_instance(ModelInstance* instance);
	void remove_instance(ModelInstance* instance);
	void clear_instances();
	void load_mesh(aiMesh* mesh, const aiScene* scene) override;
	virtual void init_instances();
	virtual void update_instances();
	void render(Shader* vertex_shader, Shader* fragment_shader);

	static void load(std::string model_path);
	static InstancedModel* get(std::string model_name);
};

#endif