// STRUCTS

struct Material
{
	sampler2D diffuse_map;
	sampler2D specular_map;
	float shininess;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cut_off;
	float outer_cut_off;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};



// IN

in vec3 vs_position;
in vec3 vs_normal;
in vec2 vs_texcoord;



// OUT

out vec4 fs_color;



// UNIFORMS

uniform vec3 camera_pos;
uniform Material material;
uniform SpotLight light;



// FUNCTION DECLARATIONS

vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir, vec3 position);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 view_dir, vec3 position);



// MAIN

void main()
{
	vec3 normal = normalize(vs_normal);
	vec3 view_dir = normalize(camera_pos - vs_position);

	vec3 result = calcSpotLight(light, normal, view_dir, vs_position);

	fs_color = vec4(result, 1.0);
}



// FUNCTION DEFINITIONS

vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.direction);

	float diff = max(dot(normal, light_dir), 0.0);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	vec3 ambient = light.ambient * texture(material.diffuse_map, vs_texcoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse_map, vs_texcoord).rgb;
	vec3 specular = light.specular * spec * texture(material.specular_map, vs_texcoord).rgb;

	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir, vec3 position)
{
	vec3 light_dir = normalize(light.position - position);

	float diff = max(dot(normal, light_dir), 0.0);
	
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	float distance = length(light.position - vs_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 ambient = light.ambient * texture(material.diffuse_map, vs_texcoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse_map, vs_texcoord).rgb;
	vec3 specular = light.specular * spec * texture(material.specular_map, vs_texcoord).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 view_dir, vec3 position)
{
	vec3 light_dir = normalize(light.position - position);

	float diff = max(dot(normal, light_dir), 0.0);
	
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	float distance = length(light.position - vs_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	float theta = dot(light_dir, normalize(-light.direction));
	float epsilon = light.cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * texture(material.diffuse_map, vs_texcoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse_map, vs_texcoord).rgb;
	vec3 specular = light.specular * spec * texture(material.specular_map, vs_texcoord).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	diffuse *= intensity;
	specular *= intensity;

	return ambient + diffuse + specular;
}