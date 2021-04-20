#include "Model.h"

Model::Model(const char* path)
{
	this->load_model(path);
}

Model::~Model()
{
	for (Mesh* mesh : this->meshes)
		delete mesh;
}

void Model::load_model(std::string path)
{
	std::cout << "LOADING MODEL" << std::endl;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
        return;
    }

    this->directory = path.substr(0, path.find_last_of('/'));
	
    this->load_node(scene->mRootNode, scene);
	std::cout << "MODEL LOADED" << std::endl;

	std::cout << "Meshes: " << this->meshes.size() << std::endl;
	std::cout << "Textures: " << this->textures_loaded.size() << std::endl;
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
	std::vector<Texture*> textures;
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
			Texture* texture;

			for (std::size_t j = 0; j < this->textures_loaded.size(); ++j)
				if (std::strcmp(this->textures_loaded[j]->get_path().c_str(), str.C_Str()) == 0)
				{
					texture = this->textures_loaded[j];
					loaded = true;
					break;
				}

			if (!loaded) {
				texture = new Texture(type, str.C_Str(), this->directory, GL_TEXTURE_2D);
				this->textures_loaded.push_back(texture);
			}

			textures.push_back(texture);
		}
	}

	this->meshes.push_back(new Mesh(vertices, indices, textures));
}

void Model::update_uniforms()
{

}

void Model::update()
{

}

void Model::render(Shader* shader)
{
	for (std::size_t i = 0; i < this->meshes.size(); ++i)
		meshes[i]->rendor(shader);
}