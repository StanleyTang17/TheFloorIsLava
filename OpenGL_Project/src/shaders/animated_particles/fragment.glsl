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

	fs_color = texture(texture_atlas, texcoord) * vec4(0.1, 0.1, 0.1, 1.0);
}