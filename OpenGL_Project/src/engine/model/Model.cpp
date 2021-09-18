#include "Model.h"

std::map<std::string, Model*> Model::create_loaded_set()
{
	if (&LOADED_SET == nullptr)
	{
		std::map<std::string, Model*> new_set;
		return new_set;
	}
	else
		return LOADED_SET;
}

std::map<std::string, Model*> Model::LOADED_SET = Model::create_loaded_set();

void Model::load_model(std::string path)
{
	Model* model = new Model(path);
	if (LOADED_SET.find(model->name) == LOADED_SET.end())
		LOADED_SET.emplace(model->name, model);
}

Model* Model::get_loaded_model(std::string model_name)
{
	if (LOADED_SET.find(model_name) != LOADED_SET.end())
		return LOADED_SET.at(model_name);
	return nullptr;
}

bool Model::remove_loaded_model(std::string model_name)
{
	if (LOADED_SET.find(model_name) != LOADED_SET.end())
		return LOADED_SET.erase(model_name);
	return false;
}

Model::Model()
{
	this->directory = "";
	this->name = "";
	this->animated = false;
}

Model::Model(std::string path)
{
	this->animated = false;
	this->load(path);
}

void Model::load(std::string path)
{
	std::size_t last_slash_pos = path.find_last_of('/');
	this->directory = path.substr(0, last_slash_pos);
	this->name = path.substr(last_slash_pos + 1, path.find_last_of('.') - last_slash_pos - 1);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return;
	}

	this->load_node(scene->mRootNode, scene);

	std::cout << this->name << " model loaded from " << path << std::endl;
}

Model::~Model()
{
	for (Mesh* mesh : this->meshes)
		delete mesh;

	for (Texture2D* texture : this->textures_loaded)
		delete texture;
}

void Model::load_node(aiNode* node, const aiScene* scene)
{
	for (std::size_t i = 0; i < node->mNumMeshes; ++i)
		this->load_mesh(scene->mMeshes[node->mMeshes[i]], scene);

	for (std::size_t i = 0; i < node->mNumChildren; ++i)
		this->load_node(node->mChildren[i], scene);
}

std::vector<Vertex> Model::load_vertices(aiMesh* mesh)
{
	std::vector<Vertex> vertices;
	for (std::size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		vertex.position = AssimpToGLM::vec3(mesh->mVertices[i]);

		if (mesh->HasNormals())
			vertex.normal = AssimpToGLM::vec3(mesh->mNormals[i]);

		if (mesh->mTextureCoords[0])
		{
			vertex.texcoord = glm::vec2(AssimpToGLM::vec3(mesh->mTextureCoords[0][i]));
			vertex.tangent = AssimpToGLM::vec3(mesh->mTangents[i]);
			vertex.bitangent = AssimpToGLM::vec3(mesh->mBitangents[i]);
		}

		vertices.push_back(vertex);
	}
	return vertices;
}

std::vector<GLuint> Model::load_indices(aiMesh* mesh)
{
	std::vector<GLuint> indices;
	for (std::size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (std::size_t j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}
	return indices;
}

std::vector<Texture2D*> Model::load_textures(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Texture2D*> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::map<aiTextureType, std::string> texture_types;
	texture_types.emplace(aiTextureType_DIFFUSE, "texture_diffuse");
	texture_types.emplace(aiTextureType_SPECULAR, "texture_specular");
	texture_types.emplace(aiTextureType_HEIGHT, "texture_normal");
	texture_types.emplace(aiTextureType_AMBIENT, "texture_height");

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
	return textures;
}

void Model::load_mesh(aiMesh* mesh, const aiScene* scene)
{
	this->meshes.push_back(new Mesh(
		this->load_vertices(mesh),
		this->load_indices(mesh),
		this->load_textures(mesh, scene)
	));
}

void Model::render(Shader* shader)
{
	for (Mesh* mesh : this->meshes)
		mesh->rendor(shader);
}