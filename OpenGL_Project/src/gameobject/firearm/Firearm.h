#pragma once

#ifndef FIREARM_H
#define FIREARM_H

#include"libs/OPENGL.h"
#include"gameobject/GameObject.h"

enum class FiringMode
{
	SEMI_AUTOMATIC, AUTOMATIC
};

class Firearm : public GameObject
{
private:
	glm::mat4 offset_from_parent;
	ModelInstance* parent;

	FiringMode firing_mode;
	int rate_of_fire; // in RPM
	float damage;
	int ammo;
	int reserve_ammo;

	float time_last_fired;

	const int MAX_AMMO;
	const int MAX_RESERVE_AMMO;

	void reload();
	void fire();

protected:
	inline void set_offset_from_parent(glm::mat4 offset) { this->offset_from_parent = offset; }

public:
	Firearm(ModelInstance* graphics_instance, glm::vec3 location, FiringMode mode, int rate_of_fire, float damage, int max_ammo, int max_reserve_ammo);

	void update() override;
	void update_mouse_input(GLFWwindow* window, int button, int action);
	void update_keyboard_input(GLFWwindow* window, int key, int action);
	inline void set_parent(ModelInstance* parent_instance) { this->parent = parent_instance; }
};

#endif
