#include "Lights.h"

std::unordered_map<LightType, std::vector<Light*>> Light::LOADED_SET = {
	{ LightType::DIRECTION, std::vector<Light*>() },
	{ LightType::POINT, std::vector<Light*>() },
	{ LightType::SPOTLIGHT, std::vector<Light*>() }
};

void Light::add(LightType type, Light* light)
{
	LOADED_SET.at(type).push_back(light);
}

void Light::send_all_to_shader(Shader* shader)
{
	std::vector<Light*> dir_lights = LOADED_SET.at(LightType::DIRECTION);
	std::vector<Light*> point_lights = LOADED_SET.at(LightType::POINT);
	std::vector<Light*> spot_lights = LOADED_SET.at(LightType::SPOTLIGHT);

	shader->set_1i(dir_lights.size(), "num_dir_lights");
	shader->set_1i(point_lights.size(), "num_point_lights");
	shader->set_1i(spot_lights.size(), "num_spot_lights");

	for (std::size_t i = 0; i < dir_lights.size(); ++i)
		dir_lights[i]->send_to_shader(shader, i);

	for (std::size_t i = 0; i < point_lights.size(); ++i)
		point_lights[i]->send_to_shader(shader, i);

	for (std::size_t i = 0; i < spot_lights.size(); ++i)
		spot_lights[i]->send_to_shader(shader, i);
}

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

DirLight::DirLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
	:
	Light(ambient, diffuse, specular)
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
	Light(ambient, diffuse, specular)
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

std::vector<PointLight*> PointLight::get_list()
{
	std::vector<Light*> lights = LOADED_SET.at(LightType::POINT);
	std::vector<PointLight*> point_lights;

	for (Light* light : lights)
		point_lights.push_back(static_cast<PointLight*>(light));

	return point_lights;
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