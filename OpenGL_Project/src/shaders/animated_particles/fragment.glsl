in vec2 vs_texcoord;
in float atlas_index;

uniform sampler2D texture_atlas;
uniform int atlas_rows;
uniform int atlas_cols;

out vec4 fs_color;

void main()
{
	int index = int(atlas_index);
	int row = index / atlas_cols;
	int col = int(mod(index, atlas_cols));

	vec2 offset = vec2(1.0 / atlas_cols * col, 1.0 / atlas_rows * row);
	vec2 texcoord = vec2(vs_texcoord.x / atlas_rows, vs_texcoord.y / atlas_cols) + offset;

	vec4 tex_color = texture(texture_atlas, texcoord);
	vec3 color_override = vec3(1.0);

	float fade = 0.5 / (atlas_rows * atlas_cols) * index;
	tex_color.a -= fade;

	if (tex_color.a < 0.0)
		tex_color.a = 0.0;
	else if (tex_color.a > 1.0)
		tex_color.a = 1.0;

	fs_color = vec4(color_override, tex_color.a);
	
}