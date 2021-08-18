in vec3 vs_texcoord;

out vec4 fs_color;

uniform samplerCube skybox_texture;

void main()
{
	fs_color = texture(skybox_texture, vs_texcoord);
}