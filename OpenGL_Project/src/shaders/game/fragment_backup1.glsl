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
	mat3 TBN;
} fs_in;



// OUT

layout (location = 0) out vec4 fs_color;
layout (location = 1) out vec4 fs_bright_color;



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
uniform sampler2D texture_height1;

uniform float height_scale;

uniform bool has_normal_map;
uniform bool has_height_map;

uniform sampler2D shadow_map;
uniform samplerCube shadow_cube;
uniform float far_plane;

uniform float gamma = 2.2;

uniform bool is_drawing_wall_light;
uniform vec2 light_tex_top_left = vec2(0.09, 0.869);
uniform vec2 light_tex_bottom_right = vec2(0.905, 0.135);

mat3 inverse_TBN = mat3(1.0);

// FUNCTION DECLARATIONS

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec2 texcoord);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec2 texcoord);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec2 texcoord);
//float calc_shadow(vec3 normal, vec3 light_dir);
float calc_point_shadow(vec3 light_pos);
vec3 gamma_to_linear(vec3 color);
vec3 linear_to_gamma(vec3 color);
bool is_light_pixel(vec2 texcoord);
bool near_light_source(vec3 highest_light_pos);
vec3 calc_normal_from_map(vec2 texcoord);
vec2 calc_texcoord_from_map(vec2 texcoord);

// MAIN

void main()
{
	inverse_TBN = transpose(fs_in.TBN);
	
	vec2 texcoord = has_height_map ? calc_texcoord_from_map(fs_in.texcoord) : fs_in.texcoord;
	vec3 normal = has_normal_map ? calc_normal_from_map(texcoord) : normalize(fs_in.normal);
	
	vec3 tangent_camera_pos = inverse_TBN * camera_pos;
	vec3 tangent_frag_pos = inverse_TBN * fs_in.position;
	vec3 view_dir = normalize(tangent_camera_pos - tangent_frag_pos);

	vec3 diffuse_color = gamma_to_linear(texture(texture_diffuse1, texcoord).rgb);
	
	if(has_height_map && (texcoord.x > 1.0 || texcoord.y > 1.0 || texcoord.x < 0.0 || texcoord.y < 0.0))
		discard;
	
	vec3 result = vec3(0.0, 0.0, 0.0);
	vec3 highest_light_pos = vec3(0.0);

	for (int i = 0; i < num_dir_lights; ++i) {
		result += calc_dir_light(dir_lights[i], normal, view_dir, diffuse_color, texcoord);
	}

	for (int i = 0; i < num_point_lights; ++i) {
		result += calc_point_light(point_lights[i], normal, view_dir, diffuse_color, texcoord);

		if (point_lights[i].position.y > highest_light_pos.y)
			highest_light_pos = point_lights[i].position;
	}

	for (int i = 0; i < num_spot_lights; ++i) {
		result += calc_spot_light(spot_lights[i], normal, view_dir, diffuse_color, texcoord);
	}

	if (is_drawing_wall_light && is_light_pixel(texcoord) && near_light_source(highest_light_pos))
		result *= vec3(100.0f);

	result = linear_to_gamma(result);

	// output bright color
	vec3 grayscale_vector = vec3(0.2126, 0.7152, 0.0722);
	float brightness = dot(result.rgb, grayscale_vector);
	if (brightness > 1.0)
		fs_bright_color = vec4(result.rgb, 1.0);
	else
		fs_bright_color = vec4(0.0, 0.0, 0.0, 1.0);

	fs_color = vec4(result, texture(texture_diffuse1, texcoord).a);
}



// FUNCTION DEFINITIONS

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec2 texcoord)
{
	vec3 light_dir = normalize(-light.direction);

	float diff = max(dot(normal, light_dir), 0.0);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	vec3 ambient = light.ambient * diffuse_color;
	vec3 diffuse = light.diffuse * diff * diffuse_color;
	vec3 specular = light.specular * spec * texture(material.specular_map, texcoord).rgb;

	return ambient + diffuse + specular;
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec2 texcoord)
{
	vec3 light_dir = normalize(inverse_TBN * light.position - inverse_TBN * fs_in.position);

	float diff = max(dot(normal, light_dir), 0.0);

	vec3 halfway_dir = normalize(light_dir + view_dir);
	float spec = pow(max(dot(normal, halfway_dir), 0.0), 2.0);

	float distance = length(inverse_TBN * light.position - inverse_TBN * fs_in.position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 ambient = light.ambient * diffuse_color;
	vec3 diffuse = light.diffuse * diff * diffuse_color;
	vec3 specular = light.specular * spec * texture(material.specular_map, texcoord).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	//return ambient + (1.0f - calc_shadow(normal, light_dir)) * (diffuse + specular);
	//return ambient + (1.0f - calc_point_shadow(light.position)) * (diffuse + specular);
	return ambient + diffuse + specular;
}

vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 view_dir, vec3 diffuse_color, vec2 texcoord)
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
	vec3 specular = light.specular * spec * texture(material.specular_map, texcoord).rgb;

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

vec3 gamma_to_linear(vec3 color)
{
    return pow(color, vec3(gamma));
}

vec3 linear_to_gamma(vec3 color)
{
    return pow(color, vec3(1.0/gamma));
}

bool is_light_pixel(vec2 texcoord)
{
	return texcoord.x > light_tex_top_left.x && texcoord.x < light_tex_bottom_right.x &&
		   texcoord.y > light_tex_bottom_right.y && texcoord.y < light_tex_top_left.y;
}

bool near_light_source(vec3 highest_light_pos)
{
	if (fs_in.position.y < highest_light_pos.y)
		return true;
	else
		return length(fs_in.position - highest_light_pos) < 3.0;
}

vec3 calc_normal_from_map(vec2 texcoord)
{
	vec3 normal = texture(texture_normal1, texcoord).rgb;
	normal = normal * 2.0 - 1.0;
	//normal = normalize(fs_in.TBN * normal);
	return normal;
}

vec2 calc_texcoord_from_map(vec2 texcoord)
{
	mat3 TBN = transpose(fs_in.TBN);

	vec3 tangent_camera_pos = TBN * camera_pos;
	vec3 tangent_frag_pos = TBN * fs_in.position;
	vec3 tangent_view_dir = normalize(tangent_camera_pos - tangent_frag_pos);

	// layer approximation
	const float num_layers = 10;
	float depth_per_layer = 1.0 / num_layers;
	vec2 texcoord_offset_per_layer = tangent_view_dir.xy / tangent_view_dir.z * height_scale / num_layers;

	float current_layer_depth = 0.0;
	vec2 current_texcoord = texcoord;
	float current_height_map_value = texture(texture_height1, current_texcoord).r;

	while (current_layer_depth < current_height_map_value)
	{
		current_layer_depth += depth_per_layer;
		current_texcoord -= texcoord_offset_per_layer;
		current_height_map_value = texture(texture_height1, current_texcoord).r;
	}

	vec2 previous_texcoord = current_texcoord + texcoord_offset_per_layer;

	float after_depth = current_height_map_value - current_layer_depth;
	float before_depth = texture(texture_height1, previous_texcoord).r - (current_layer_depth - depth_per_layer);

	float weight = after_depth / (after_depth - before_depth);
	
	return previous_texcoord * weight + current_texcoord * (1.0 - weight);
}