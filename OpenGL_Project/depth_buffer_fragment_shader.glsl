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
	vec4 light_space_position;
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



// FUNCTION DECLARATIONS

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color, float shadow);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color);
float calc_shadow();



// MAIN

void main()
{
	// gl_FragDepth = gl_FragCoord.z;
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

vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color, float shadow)
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

	return ambient + (1.0f - shadow) * (diffuse + specular);
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

float calc_shadow()
{
	vec3 proj_coords = fs_in.light_space_position.xyz / fs_in.light_space_position.w;
	proj_coords = proj_coords * 0.5 + 0.5;
	
	float closest_depth = texture(shadow_map, proj_coords.xy).r;
	float current_depth = proj_coords.z;
	float shadow = current_depth > closest_depth ? 1.0f : 0.0f;

	return shadow;
}