#include "Model.h"

Model::Model(const char* path)
{
	this->load_model(path);
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);
	this->update_model_matrix();
}

Model::~Model()
{
	for (Mesh* mesh : this->meshes)
		delete mesh;
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

void Model::set_position(glm::vec3 position)
{
	this->position = position;
}

void Model::set_rotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void Model::set_scale(glm::vec3 scale)
{
	this->scale = scale;
}

void Model::move(glm::vec3 translate)
{
	this->position += translate;
}

void Model::rotate(glm::vec3 rotate)
{
	this->rotation += rotate;
}

void Model::_scale(glm::vec3 scale)
{
	this->scale *= scale;
}

void Model::update_model_matrix()
{
	this->model_matrix = glm::mat4(1.0f);
	this->model_matrix = glm::translate(this->model_matrix, this->position);
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	this->model_matrix = glm::scale(this->model_matrix, this->scale);
}

void Model::update_uniforms(Shader* shader)
{
	shader->set_mat_4fv(this->model_matrix, "model_matrix", GL_FALSE);
}

void Model::update(float dt)
{
	for (Mesh* mesh : this->meshes)
	{
		mesh->update(dt);
	}
}

void Model::render(Shader* shader)
{
	this->update_model_matrix();
	this->update_uniforms(shader);
	for (std::size_t i = 0; i < this->meshes.size(); ++i)
		meshes[i]->rendor(shader);
}