#include "Model.h"

Model::Model(const char* path)
{
	this->load_model(path);
	glGenBuffers(1, &this->instance_VBO);
}

Model::~Model()
{
	for (Mesh* mesh : this->meshes)
		delete mesh;

	for (ModelInstance* instance : this->instances)
		delete instance;
}

void Model::load_model(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return;
	}

	this->directory = path.substr(0, path.find_last_of('/'));

	this->load_node(scene->mRootNode, scene);

	std::cout << "Model loaded from " << path << std::endl;
}

void Model::load_node(aiNode* node, const aiScene* scene)
{
	for (std::size_t i = 0; i < node->mNumMeshes; ++i)
		this->load_mesh(scene->mMeshes[node->mMeshes[i]], scene);

	for (std::size_t i = 0; i < node->mNumChildren; ++i)
		this->load_node(node->mChildren[i], scene);
}

void Model::load_mesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture2D*> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::map<aiTextureType, std::string> texture_types;
	texture_types.emplace(aiTextureType_DIFFUSE, "texture_diffuse");
	texture_types.emplace(aiTextureType_SPECULAR, "texture_specular");
	texture_types.emplace(aiTextureType_HEIGHT, "texture_normal");
	texture_types.emplace(aiTextureType_AMBIENT, "texture_height");

	for (std::size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		aiVector3D aiVertex = mesh->mVertices[i];

		Vertex vertex;
		vertex.position = glm::vec3(aiVertex.x, aiVertex.y, aiVertex.z);

		if (mesh->HasNormals())
		{
			aiVector3D aiNormal = mesh->mNormals[i];
			vertex.normal = glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z);
		}

		if (mesh->mTextureCoords[0])
		{
			aiVector3D aiTexcoord = mesh->mTextureCoords[0][i];
			aiVector3D aiTangent = mesh->mTangents[i];
			aiVector3D aiBitangent = mesh->mBitangents[i];
			vertex.texcoord = glm::vec2(aiTexcoord.x, aiTexcoord.y);
			vertex.tangent = glm::vec3(aiTangent.x, aiTangent.y, aiTangent.z);
			vertex.bitangent = glm::vec3(aiBitangent.x, aiBitangent.y, aiBitangent.z);
		}

		vertices.push_back(vertex);
	}

	for (std::size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (std::size_t j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	for (std::map<aiTextureType, std::string>::iterator it = texture_types.begin(); it != texture_types.end(); ++it)
	{
		aiTextureType aiType = it->first;
		std::string type = it->second;
		for (std::size_t i = 0; i < material->GetTextureCount(aiType); ++i)
		{
			aiString str;
			material->GetTexture(aiType, i, &str);
			
			bool loaded = false;
			Texture2D* texture = nullptr;

			for (std::size_t j = 0; j < this->textures_loaded.size(); ++j)
				if (std::strcmp(this->textures_loaded[j]->get_path().c_str(), str.C_Str()) == 0)
				{
					texture = this->textures_loaded[j];
					loaded = true;
					break;
				}

			if (!loaded) {
				texture = new Texture2D(type, str.C_Str(), this->directory);
				this->textures_loaded.push_back(texture);
			}

			textures.push_back(texture);
		}
	}

	this->meshes.push_back(new Mesh(vertices, indices, textures));
}

void Model::init_instances()
{
	std::vector<glm::mat4> model_matrices;
	for (ModelInstance* instance : instances)
		model_matrices.push_back(instance->get_model_matrix());

	glBindBuffer(GL_ARRAY_BUFFER, this->instance_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * model_matrices.size(), model_matrices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (Mesh* mesh : this->meshes)
		mesh->init_instances(this->instance_VBO);
}

void Model::add_instance(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->instances.push_back(new ModelInstance(position, rotation, scale));
}

void Model::add_instance(ModelInstance* instance)
{
	this->instances.push_back(instance);
}

void Model::remove_instance(int index)
{
	if (index > -1)
		this->instances.erase(this->instances.begin() + index);
	else
		this->instances.pop_back();
}

void Model::remove_instance(ModelInstance* instance)
{
	for(std::size_t i = 0; i < this->instances.size(); ++i)
		if (instances[i] == instance)
		{
			instances.erase(instances.begin() + i);
			break;
		}
}

ModelInstance* Model::get_instance(std::size_t index)
{
	return this->instances[index];
}

void Model::update(float dt)
{
	this->init_instances();
	for (Mesh* mesh : this->meshes)
	{
		mesh->update(dt);
	}
}

void Model::render(Shader* shader)
{
	for (std::size_t i = 0; i < this->meshes.size(); ++i)
		meshes[i]->rendor(shader, this->instances.size());
}