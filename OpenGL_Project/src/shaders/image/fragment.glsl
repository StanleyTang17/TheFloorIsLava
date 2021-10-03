in vec2 texcoords;
out vec4 color;

uniform sampler2D image_texture;

void main()
{
	color = texture(image_texture, texcoords);
}