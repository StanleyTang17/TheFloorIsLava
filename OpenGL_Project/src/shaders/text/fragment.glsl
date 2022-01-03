in vec2 texcoords;
out vec4 color;

uniform sampler2D font_texture;
uniform vec4 text_color;
uniform vec4 outline_color;

void main()
{
	vec4 tex_color = texture(font_texture, texcoords);
	float fill = tex_color.r;
	float outline = tex_color.g;
	float alpha = max(fill, outline);

	vec4 mix_color = mix(vec4(0.0), text_color, fill);
	mix_color = mix(mix_color, outline_color, outline);

	color = mix_color;
}