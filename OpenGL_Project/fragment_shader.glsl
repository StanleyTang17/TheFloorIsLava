// CONSTANTS

#define MAX_LIGHTS 64



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

in GS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} fs_in;



// OUT

out vec4 fs_color;



// UNIFORMS

uniform vec3 camera_pos;
uniform Material material;

uniform DirLight dir_lights[MAX_LIGHTS];
uniform PointLight point_lights[MAX_LIGHTS];
uniform SpotLight spot_lights[MAX_LIGHTS];

uniform int num_dir_lights;
uniform int num_point_lights;
uniform int num_spot_lights;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;



// FUNCTION DECLARATIONS

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color);



// MAIN

void main()
{
	vec3 normal = normalize(fs_in.normal);
	vec3 view_dir = normalize(camera_pos - fs_in.position);

	vec3 result = vec3(0.0, 0.0, 0.0);
	float gamma = 2.2;
	vec3 diffuse_color = pow(texture(material.diffuse_map, fs_in.texcoord).rgb, vec3(gamma));

	for (int i = 0; i < num_dir_lights; ++i) {
		result += calc_dir_light(dir_lights[i], normal, view_dir, diffuse_color);
	}

	for (int i = 0; i < num_point_lights; ++i) {
		result += calc_point_light(point_lights[i], normal, view_dir, diffuse_color);
	}

	for (int i = 0; i < num_spot_lights; ++i) {
		result += calc_spot_light(spot_lights[i], normal, view_dir, diffuse_color);
	}

	fs_color = vec4(result, texture(texture_diffuse1, fs_in.texcoord).a);

	//fs_color = texture(texture_diffuse1, fs_in.texcoord) * vec4(vec3(gl_FragCoord.z), 1.0);
}



// FUNCTION DEFINITIONS

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color)
{
	vec3 light_dir = normalize(-light.direction);

	float diff = max(dot(normal, light_dir), 0.0);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	vec3 ambient = light.ambient * diffuse_color;
	vec3 diffuse = light.diffuse * diff * diffuse_color;
	vec3 specular = light.specular * spec * texture(material.specular_map, fs_in.texcoord).rgb;

	return ambient + diffuse + specular;
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color)
{
	vec3 light_dir = normalize(light.position - fs_in.position);

	float diff = max(dot(normal, light_dir), 0.0);

	vec3 halfway_dir = normalize(light_dir + view_dir);
	float spec = pow(max(dot(normal, halfway_dir), 0.0), 2.0);

	float distance = length(light.position - fs_in.position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 ambient = light.ambient * diffuse_color;
	vec3 diffuse = light.diffuse * diff * diffuse_color;
	vec3 specular = light.specular * spec * texture(material.specular_map, fs_in.texcoord).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color)
{
	vec3 light_dir = normalize(light.position - fs_in.position);

	float diff = max(dot(normal, light_dir), 0.0);
	
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	float distance = length(light.position - fs_in.position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	float theta = dot(light_dir, normalize(-light.direction));
	float epsilon = light.cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * diffuse_color;
	vec3 diffuse = light.diffuse * diff * diffuse_color;
	vec3 specular = light.specular * spec * texture(material.specular_map, fs_in.texcoord).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	diffuse *= intensity;
	specular *= intensity;

	return ambient + diffuse + specular;
}