#include "InstancedModel.h"

InstancedModel::InstancedModel(std::string model_path)
	:
	Model(model_path)
{
	glGenBuffers(1, &this->instance_buffer);
}

InstancedModel::~InstancedModel()
{
	for (ModelInstance* instance : this->instances)
		delete instance;
}

void InstancedModel::load_mesh(aiMesh* mesh, const aiScene* scene)
{
	Mesh* new_mesh = new Mesh(
		this->load_vertices(mesh),
		this->load_indices(mesh),
		this->load_textures(mesh, scene)
	);

	this->meshes.push_back(new_mesh);
}

void InstancedModel::render(Shader* vertex_shader, Shader* fragment_shader)
{
	this->update_instances();
	for (Mesh* mesh : this->meshes)
		mesh->rendor(vertex_shader, fragment_shader, this->instances.size());
}

void InstancedModel::init_instances()
{
	std::vector<glm::mat4> matrices;
	for (ModelInstance* instance : this->instances)
		matrices.push_back(instance->get_model_matrix());

	glBindBuffer(GL_ARRAY_BUFFER, this->instance_buffer);
	glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	for (Mesh* new_mesh : this->meshes)
	{
		
		glBindVertexArray(new_mesh->get_VAO());
		//new_mesh->init_instanced_mat4(5, this->instance_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->instance_buffer);

		std::size_t vec4_size = sizeof(glm::vec4);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)0);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(1 * vec4_size));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(2 * vec4_size));
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(3 * vec4_size));

		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void InstancedModel::update_instances()
{
	std::vector<glm::mat4> matrices;
	for (ModelInstance* instance : this->instances)
		matrices.push_back(instance->get_model_matrix());

	glBindBuffer(GL_ARRAY_BUFFER, this->instance_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, matrices.size() * sizeof(glm::mat4), matrices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedModel::add_instance(ModelInstance* instance)
{
	this->instances.push_back(instance);
}

void InstancedModel::remove_instance(ModelInstance* instance)
{
	for (std::vector<ModelInstance*>::iterator it = this->instances.begin(); it != this->instances.end(); ++it)
		if (*it == instance)
		{
			this->instances.erase(it);
			break;
		}
}

void InstancedModel::clear_instances()
{
	this->instances.clear();
}

void InstancedModel::load(std::string model_path)
{
	InstancedModel* model = new InstancedModel(model_path);
	if (LOADED_SET.find(model->name) == LOADED_SET.end())
		LOADED_SET.emplace(model->name, model);
}

InstancedModel* InstancedModel::get(std::string model_name)
{
	if (LOADED_SET.find(model_name) != LOADED_SET.end())
		return static_cast<InstancedModel*>(LOADED_SET.at(model_name));
	return nullptr;
}