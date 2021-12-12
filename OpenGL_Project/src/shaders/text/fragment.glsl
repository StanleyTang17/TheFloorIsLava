in vec2 texcoords;
out vec4 color;

uniform sampler2D font_texture;
uniform vec4 font_color;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(font_texture, texcoords).r);
	color = font_color * sampled;
}