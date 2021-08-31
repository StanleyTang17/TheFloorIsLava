#include"AnimationSequence.h"

Animation::Sequence::Sequence(std::string animation_path, Animation::Model* model, std::size_t animation_index)
{
	this->duration = 0.0f;
	this->ticks_per_second = 0;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animation_path, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return;
	}

	aiAnimation* animation = scene->mAnimations[animation_index];

	this->duration = animation->mDuration;
	this->ticks_per_second = animation->mTicksPerSecond;

	this->nodes.push_back(AssimpNodeData());
	this->read_heirarchy_data(0, scene->mRootNode);
	this->read_missing_bones(animation, model);
}

Animation::Sequence::~Sequence()
{
	for (Bone* bone : this->bones)
		delete bone;
}

void Animation::Sequence::read_heirarchy_data(std::size_t index, const aiNode* src)
{
	assert(src);

	this->nodes[index].name = src->mName.C_Str();
	this->nodes[index].transformation = AssimpToGLM::mat4(src->mTransformation);

	for (std::size_t i = 0; i < src->mNumChildren; ++i)
	{
		std::size_t new_index = this->nodes.size();
		this->nodes.push_back(AssimpNodeData());
		this->read_heirarchy_data(new_index, src->mChildren[i]);
		this->nodes[index].children_indices.push_back(new_index);
	}
}

void Animation::Sequence::read_missing_bones(const aiAnimation* animation, Animation::Model* model)
{
	std::map<std::string, BoneInfo> bone_map = model->get_bone_info_map();
	int bone_count = model->get_bone_count();

	for (std::size_t i = 0; i < animation->mNumChannels; ++i)
	{
		aiNodeAnim* channel = animation->mChannels[i];
		std::string bone_name = channel->mNodeName.C_Str();

		if (bone_map.find(bone_name) == bone_map.end())
		{
			bone_map[bone_name].id = bone_count;
			std::cout << "Found missing bone: " << bone_count << std::endl;
			++bone_count;
		}

		this->bones.push_back(new Bone(bone_name, bone_map.at(bone_name).id, channel));
	}

	this->bone_info_map = bone_map;
}

Animation::Bone* Animation::Sequence::find_bone(std::string name)
{
	for (Bone* bone : this->bones)
		if (bone->get_name() == name)
			return bone;

	return nullptr;
}