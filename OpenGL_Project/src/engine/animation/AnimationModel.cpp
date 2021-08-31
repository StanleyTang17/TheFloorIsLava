#include"AnimationModel.h"

Animation::Model::Model(std::string path)
{
	this->load_model(path);
}

Animation::Model::~Model()
{
	for (Mesh* mesh : this->meshes)
		delete mesh;

	for (Texture2D* texture : this->textures_loaded)
		delete texture;
}

void Animation::Model::load_model(std::string path)
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

	std::cout << "Animated Model loaded from " << path << std::endl;
}

void Animation::Model::load_node(aiNode* node, const aiScene* scene)
{
	for (std::size_t i = 0; i < node->mNumMeshes; ++i)
		this->load_mesh(scene->mMeshes[node->mMeshes[i]], scene);

	for (std::size_t i = 0; i < node->mNumChildren; ++i)
		this->load_node(node->mChildren[i], scene);
}

void Animation::Model::set_vertex_bone_default(Vertex& vertex)
{
	for (std::size_t i = 0; i < 4; ++i)
	{
		vertex.bone_ids[i] = -1;
		vertex.weights[i] = 0.0;
	}
}

void Animation::Model::append_vertex_bone(Vertex& vertex, int bone_id, float weight)
{
	for (std::size_t i = 0; i < 4; ++i)
	{
		if (vertex.bone_ids[i] < 0)
		{
			vertex.bone_ids[i] = bone_id;
			vertex.weights[i] = weight;
			break;
		}
	}
}

void Animation::Model::extract_bone_info(std::vector<Vertex> &vertices, aiMesh* mesh, const aiScene* scene)
{
	for (std::size_t i = 0; i < mesh->mNumBones; ++i)
	{
		aiBone* bone = mesh->mBones[i];
		std::string bone_name = bone->mName.C_Str();
		int bone_id = -1;

		if (this->bone_info_map.find(bone_name) == this->bone_info_map.end())
		{
			bone_id = this->bone_count;
			BoneInfo info = { bone_id, AssimpToGLM::mat4(bone->mOffsetMatrix) };
			this->bone_info_map.emplace(bone_name, info);
			this->bone_count++;
		}
		else
		{
			bone_id = this->bone_info_map.at(bone_name).id;
		}

		assert(bone_id != -1);

		for (std::size_t j = 0; j < bone->mNumWeights; ++j)
		{
			int vertex_id = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;
			assert(vertex_id <= vertices.size());
			this->append_vertex_bone(vertices[vertex_id], bone_id, weight);
		}
	}
}

void Animation::Model::load_mesh(aiMesh* mesh, const aiScene* scene)
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
		Vertex vertex;
		this->set_vertex_bone_default(vertex);
		vertex.position = AssimpToGLM::vec3(mesh->mVertices[i]);

		if (mesh->HasNormals())
		{
			vertex.normal = AssimpToGLM::vec3(mesh->mNormals[i]);
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.texcoord = glm::vec2(AssimpToGLM::vec3(mesh->mTextureCoords[0][i]));
			vertex.tangent = AssimpToGLM::vec3(mesh->mTangents[i]);
			vertex.bitangent = AssimpToGLM::vec3(mesh->mBitangents[i]);
		}

		vertices.push_back(vertex);
	}

	this->extract_bone_info(vertices, mesh, scene);

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

void Animation::Model::update(const float dt)
{
	for (Mesh* mesh : this->meshes)
	{
		mesh->update(dt);
	}
}

void Animation::Model::render(Shader* shader)
{
	for (Mesh* mesh : this->meshes)
	{
		mesh->rendor(shader);
	}
}