in vec2 texcoords;
out vec4 color;

uniform sampler2D image_texture;
uniform float alpha_override;

void main()
{
	if (alpha_override < 0.0)
		color = texture(image_texture, texcoords);
	else
		color = vec4(texture(image_texture, texcoords).rgb, alpha_override);
}