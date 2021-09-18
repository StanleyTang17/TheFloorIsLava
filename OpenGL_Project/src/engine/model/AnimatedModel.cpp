#include "AnimatedModel.h"

void AnimatedModel::load_model(std::string model_path, std::string split_animation_path)
{
	AnimatedModel* model = new AnimatedModel(model_path, split_animation_path);
	if (LOADED_SET.find(model->name) == LOADED_SET.end())
		LOADED_SET.emplace(model->name, model);
}

AnimatedModel::AnimatedModel(std::string model_path, std::string split_animation_path)
	:
	Model(model_path)
{
	this->bone_count = 0;
	this->animated = true;
	this->FPS = 24;
	this->bone_matrices.reserve(100);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(model_path, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return;
	}

	aiAnimation* animation = scene->mAnimations[0];
	this->FPS = animation->mTicksPerSecond;

	this->read_animated_node(0, scene->mRootNode);
	this->read_animated_bones(animation, scene);
	this->read_split_animations(split_animation_path);

}

void AnimatedModel::set_vertex_bone_default(AnimatedVertex& vertex)
{
	for (std::size_t i = 0; i < 4; ++i)
	{
		vertex.bone_ids[i] = -1;
		vertex.weights[i] = 0.0;
	}
}

void AnimatedModel::append_vertex_bone(AnimatedVertex& vertex, int bone_id, float weight)
{
	for (std::size_t i = 0; i < 4; ++i)
		if (vertex.bone_ids[i] < 0)
		{
			vertex.bone_ids[i] = bone_id;
			vertex.weights[i] = weight;
			break;
		}
}

void AnimatedModel::extract_bone_info(std::vector<AnimatedVertex>& vertices, aiMesh* mesh, const aiScene* scene)
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
			bone_id = this->bone_info_map.at(bone_name).id;

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

void AnimatedModel::read_animated_node(std::size_t index, const aiNode* node)
{
	assert(node);

	if (this->animated_nodes.size() == index)
		this->animated_nodes.push_back(AnimatedNode());

	this->animated_nodes[index].name = node->mName.C_Str();
	this->animated_nodes[index].transformation = AssimpToGLM::mat4(node->mTransformation);

	for (std::size_t i = 0; i < node->mNumChildren; ++i)
	{
		std::size_t new_index = this->animated_nodes.size();
		this->read_animated_node(new_index, node->mChildren[i]);
		this->animated_nodes[index].children_indices.push_back(new_index);
	}
}

void AnimatedModel::read_animated_bones(aiAnimation* animation, const aiScene* scene)
{
	for (std::size_t i = 0; i < animation->mNumChannels; ++i)
	{
		aiNodeAnim* channel = animation->mChannels[i];
		std::string bone_name = channel->mNodeName.C_Str();

		if (this->bone_info_map.find(bone_name) == this->bone_info_map.end())
		{
			BoneInfo new_bone = { this->bone_count, glm::mat4(1.0f) };
			this->bone_info_map.emplace(bone_name, new_bone);
			std::cout << "Found missing bone: " << this->bone_count << std::endl;
			++this->bone_count;
		}

		this->animated_bones.push_back(AnimatedBone(bone_name, this->bone_info_map.at(bone_name).id, channel));
	}
}

void AnimatedModel::read_split_animations(std::string split_file)
{
	std::string line = "";
	std::fstream in_file;

	in_file.open(split_file);
	if (in_file.is_open())
	{
		while (std::getline(in_file, line))
		{
			std::stringstream ss(line);
			std::string name, start_frame, end_frame;
			ss >> name;
			ss >> start_frame;
			ss >> end_frame;
			Sequence animation = { stoi(start_frame) * this->FPS, stoi(end_frame) * this->FPS };
			this->animations.emplace(name, animation);
		}
	}
	else
		std::cout << "CANNOT READ SPLIT ANIMATION" << std::endl;

}

AnimatedBone* AnimatedModel::find_bone_by_name(std::string name)
{
	for (AnimatedBone bone : this->animated_bones)
		if (bone.get_name() == name)
			return &bone;

	return nullptr;
}

void AnimatedModel::calculate_bone_transform(std::size_t index, glm::mat4 parent_transform, const float animation_time)
{
	AnimatedNode node = this->animated_nodes[index];
	std::string node_name = node.name;
	glm::mat4 node_transform = node.transformation;

	AnimatedBone* bone = this->find_bone_by_name(node_name);

	if (bone != nullptr)
	{
		bone->update(animation_time);
		node_transform = bone->get_transform();
	}

	glm::mat4 final_transform = parent_transform * node_transform;

	if (this->bone_info_map.find(node_name) != this->bone_info_map.end())
	{
		BoneInfo bone_info = this->bone_info_map.at(node_name);
		this->bone_matrices[bone_info.id] = final_transform * bone_info.offset;
	}

	for (std::size_t i = 0; i < node.children_indices.size(); ++i)
		this->calculate_bone_transform(node.children_indices[i], final_transform, animation_time);
}

std::vector<glm::mat4> AnimatedModel::get_bone_matrices(const float animation_time)
{
	this->calculate_bone_transform(0, glm::mat4(1.0f), animation_time);
	return this->bone_matrices;
}

std::vector<AnimatedVertex> AnimatedModel::load_vertices(aiMesh* mesh, const aiScene* scene)
{
	std::vector<AnimatedVertex> vertices;
	for (std::size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		AnimatedVertex vertex;
		this->set_vertex_bone_default(vertex);
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

	this->extract_bone_info(vertices, mesh, scene);

	return vertices;
}

void AnimatedModel::load_mesh(aiMesh* mesh, const aiScene* scene)
{
	this->meshes.push_back(new Mesh(
		this->load_vertices(mesh, scene),
		this->load_indices(mesh),
		this->load_textures(mesh, scene)
	));
}