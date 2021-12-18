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

in VS_OUT
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

uniform sampler2D shadow_map;
uniform samplerCube shadow_cube;
uniform float far_plane;

uniform float exposure;


// FUNCTION DECLARATIONS

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color);
//float calc_shadow(vec3 normal, vec3 light_dir);
float calc_point_shadow(vec3 light_pos);



// MAIN

void main()
{
	vec3 normal = normalize(fs_in.normal);
	vec3 view_dir = normalize(camera_pos - fs_in.position);

	vec3 result = vec3(0.0, 0.0, 0.0);
	float gamma = 2.2;
	vec3 diffuse_color = texture(material.diffuse_map, fs_in.texcoord).rgb;
	
	// transform from gamma space to linear space
	diffuse_color = pow(diffuse_color, vec3(gamma));

	for (int i = 0; i < num_dir_lights; ++i) {
		result += calc_dir_light(dir_lights[i], normal, view_dir, diffuse_color);
	}

	for (int i = 0; i < num_point_lights; ++i) {
		result += calc_point_light(point_lights[i], normal, view_dir, diffuse_color);
	}

	for (int i = 0; i < num_spot_lights; ++i) {
		result += calc_spot_light(spot_lights[i], normal, view_dir, diffuse_color);
	}
	
	// Reinhard tone mapping
	//result = result / (result + vec3(1.0));

	// exposure tone mapping
	result = vec3(1.0) - exp(-result * exposure);

	// transform from linear space back to gamma space
	result = pow(result, vec3(1.0/gamma));

	fs_color = vec4(result, texture(texture_diffuse1, fs_in.texcoord).a);
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

	//return ambient + (1.0f - calc_shadow(normal, light_dir)) * (diffuse + specular);
	return ambient + (1.0f - calc_point_shadow(light.position)) * (diffuse + specular);
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
/*
float calc_shadow(vec3 normal, vec3 light_dir)
{
	vec3 proj_coords = fs_in.light_space_position.xyz / fs_in.light_space_position.w;
	proj_coords = proj_coords * 0.5 + 0.5;
	
	float closest_depth = texture(shadow_map, proj_coords.xy).r;
	float current_depth = proj_coords.z;

	float bias = max(0.05 * (1 - dot(normal, light_dir)), 0.005);

	float shadow = 0.0;
	vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
	int num_samples = 0;
	for(int x = -2; x <= 2; ++x) {
		for(int y = -2; y <= 2; ++y) {
			float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x,y) * texel_size).r;
			shadow += current_depth - bias > pcf_depth ? 1.0f : 0.0f;
			num_samples += 1;
		}
	}

	shadow /= num_samples;

	if(proj_coords.z > 1.0)
		shadow = 0.0;

	return shadow;
}
*/
float calc_point_shadow(vec3 light_pos)
{
	vec3 pos_to_light = fs_in.position - light_pos;
	float closest_depth = texture(shadow_cube, pos_to_light).r * far_plane;
	float current_depth = length(pos_to_light);

	float bias = 0.05f;
	float shadow = current_depth - bias > closest_depth ? 1.0f : 0.0f;

	return shadow;
}