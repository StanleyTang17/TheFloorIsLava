#pragma once

#ifndef LIGHTS_H
#define LIGHTS_H

#include"engine/shader/Shader.h"

enum class LightType
{
	DIRECTION, POINT, SPOTLIGHT
};

class Light
{
protected:
	static std::unordered_map<LightType, std::vector<Light*>> LOADED_SET;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	virtual void send_to_shader(Shader* shader, const int index) = 0;

	const glm::vec3 get_ambient() const { return this->ambient; }
	const glm::vec3 get_diffuse() const { return this->diffuse; }
	const glm::vec3 get_specular() const { return this->specular; }

	void set_ambient(glm::vec3 ambient) { this->ambient = ambient; }
	void set_diffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
	void set_specular(glm::vec3 specular) { this->specular = specular; }

	static void add(LightType type, Light* light);
	static void send_all_to_shader(Shader* shader);
};

class DirLight : public Light
{
protected:
	glm::vec3 direction;
public:
	DirLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
	void send_to_shader(Shader* shader, const int index);

	const glm::vec3 get_direction() const { return this->direction; }

	void set_direction(glm::vec3 directon) { this->direction = direction; }
};

class PointLight : public Light
{
protected:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;
public:
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, const float constant, const float linear, const float quadratic);
	void send_to_shader(Shader* shader, const int index);

	const glm::vec3 get_position() const { return this->position; }

	void set_position(glm::vec3 position) { this->position = position; }

	static std::vector<PointLight*> get_list();
};

class SpotLight : public PointLight
{
protected:
	glm::vec3 direction;

	float cut_off;
	float outer_cut_off;
public:
	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, const float constant, const float linear, const float quadratic, float cut_off, float outer_cut_off);
	void send_to_shader(Shader* shader, const int index);

	const glm::vec3 get_direction() const { return this->direction; }

	void set_direction(glm::vec3 direction) { this->direction = direction; }
};

#endif