#pragma once

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include"AnimationSequence.h"
#include<math.h>

class Animation::Animator
{
private:
	Animation::Sequence* current_animation;
	std::vector<glm::mat4> final_bone_matrices;
	float current_time;

	void calculate_bone_transform(std::size_t index, glm::mat4 parent_transform);
	
public:
	Animator(Animation::Sequence* animation);

	void update(const float dt);
	void play_animation(Animation::Sequence* animation);
	std::vector<glm::mat4> get_final_bone_matrices() { return this->final_bone_matrices; }
	float get_animation_time() const { return this->current_time; }
};

#endif