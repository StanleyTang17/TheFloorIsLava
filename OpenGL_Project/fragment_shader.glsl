

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuse_texture;
	sampler2D specular_texture;
};

// IN
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;


// OUT
out vec4 fs_color;


// UNIFORMS
uniform vec3 light_pos0;
uniform vec3 camera_pos;
uniform Material material;


// FUNCTIONS
vec3 calculate_ambient(Material material)
{
	return material.ambient;
}

vec3 calculate_diffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 light_pos0)
{
	vec3 pos_to_light_dir_vec = normalize(light_pos0 - vs_position);
	float diffuse = clamp(dot(pos_to_light_dir_vec, vs_normal), 0, 1);
	vec3 diffuse_final = material.diffuse * diffuse;

	return diffuse_final;
}

vec3 calculate_specular(Material material, vec3 vs_position, vec3 vs_normal, vec3 light_pos0, vec3 camera_pos)
{
	vec3 light_to_pos_dir_vec = normalize(vs_position - light_pos0);
	vec3 reflect_dir_vec = normalize(reflect(light_to_pos_dir_vec, normalize(vs_normal)));
	vec3 pos_to_view_dir_vec = normalize(camera_pos - vs_position);
	float specular_constant = pow(max(dot(pos_to_view_dir_vec, reflect_dir_vec), 0), 100);
	vec3 specular_final = material.specular * specular_constant * vec3(texture(material.specular_texture, vs_texcoord));
	// vec3 specular_final = material.specular * specular_constant;

	return specular_final;
}


void main()
{
	// fs_color = vec4(vs_color, 1.f);

	// LIGHTING
	vec3 ambient_final = calculate_ambient(material);
	vec3 diffuse_final = calculate_diffuse(material, vs_position, vs_normal, light_pos0);
	vec3 specular_final = calculate_specular(material, vs_position, vs_normal, light_pos0, camera_pos);

	// ATTENUATION

	fs_color = texture(material.diffuse_texture, vs_texcoord) * (vec4(ambient_final, 1.0f) + vec4(diffuse_final, 1.0f) + vec4(specular_final, 1.0f));
}