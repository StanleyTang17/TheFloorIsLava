#include "Lights.h"

AbstractLight::AbstractLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

DirLight::DirLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
	:
	AbstractLight(ambient, diffuse, specular)
{
	this->direction = direction;
}

void DirLight::send_to_shader(Shader* shader, const int index)
{
	const char * array = "dir_lights";
	char buffer[64];

	sprintf_s(buffer, "%s[%d].ambient", array, index);
	shader->set_vec_3f(this->ambient, buffer);

	sprintf_s(buffer, "%s[%d].diffuse", array, index);
	shader->set_vec_3f(this->diffuse, buffer);

	sprintf_s(buffer, "%s[%d].specular", array, index);
	shader->set_vec_3f(this->specular, buffer);

	sprintf_s(buffer, "%s[%d].direction", array,  index);
	shader->set_vec_3f(this->direction, buffer);
}

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, const float constant, const float linear, const float quadratic)
	:
	AbstractLight(ambient, diffuse, specular)
{
	this->position = position;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void PointLight::send_to_shader(Shader* shader, const int index)
{
	const char* array = "point_lights";
	char buffer[64];

	sprintf_s(buffer, "%s[%d].ambient", array, index);
	shader->set_vec_3f(this->ambient, buffer);

	sprintf_s(buffer, "%s[%d].diffuse", array, index);
	shader->set_vec_3f(this->diffuse, buffer);

	sprintf_s(buffer, "%s[%d].specular", array, index);
	shader->set_vec_3f(this->specular, buffer);

	sprintf_s(buffer, "%s[%d].position", array, index);
	shader->set_vec_3f(this->position, buffer);

	sprintf_s(buffer, "%s[%d].constant", array, index);
	shader->set_1f(this->constant, buffer);

	sprintf_s(buffer, "%s[%d].linear", array, index);
	shader->set_1f(this->linear, buffer);

	sprintf_s(buffer, "%s[%d].quadratic", array, index);
	shader->set_1f(this->quadratic, buffer);
}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, const float constant, const float linear, const float quadratic, float cut_off, float outer_cut_off)
	:
	PointLight(ambient, diffuse, specular, position, constant, linear, quadratic)
{
	this->direction = direction;
	this->cut_off = cut_off;
	this->outer_cut_off = outer_cut_off;
}

void SpotLight::send_to_shader(Shader* shader, const int index)
{
	const char* array = "spot_lights";
	char buffer[64];

	sprintf_s(buffer, "%s[%d].ambient", array, index);
	shader->set_vec_3f(this->ambient, buffer);

	sprintf_s(buffer, "%s[%d].diffuse", array, index);
	shader->set_vec_3f(this->diffuse, buffer);

	sprintf_s(buffer, "%s[%d].specular", array, index);
	shader->set_vec_3f(this->specular, buffer);

	sprintf_s(buffer, "%s[%d].position", array, index);
	shader->set_vec_3f(this->position, buffer);

	sprintf_s(buffer, "%s[%d].direction", array, index);
	shader->set_vec_3f(this->direction, buffer);

	sprintf_s(buffer, "%s[%d].constant", array, index);
	shader->set_1f(this->constant, buffer);

	sprintf_s(buffer, "%s[%d].linear", array, index);
	shader->set_1f(this->linear, buffer);

	sprintf_s(buffer, "%s[%d].quadratic", array, index);
	shader->set_1f(this->quadratic, buffer);

	sprintf_s(buffer, "%s[%d].cut_off", array, index);
	shader->set_1f(glm::cos(glm::radians(this->cut_off)), buffer);

	sprintf_s(buffer, "%s[%d].outer_cut_off", array, index);
	shader->set_1f(glm::cos(glm::radians(this->outer_cut_off)), buffer);
}