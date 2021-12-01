in vec2 texcoords;

out vec4 color;

uniform float time;
uniform float alpha;
uniform sampler2D image_texture;
uniform sampler2D flowmap_texture;

vec2 bound(vec2 coord);
vec4 calculate_color(vec2 texcoords, float time);

void main()
{
	vec4 color1 = calculate_color(texcoords, time);
	vec4 color2 = calculate_color(texcoords, time + 0.5);
	color = color1 + color2;
}

vec4 calculate_color(vec2 texcoords, float time)
{
	time = fract(time);

	vec4 flowmap_color = texture(flowmap_texture, texcoords);
	vec2 velocity = vec2((flowmap_color.r - 0.5) * 2, (flowmap_color.g - 0.5) * 2);
	float weight = 1.0 - abs(1 - 2 * time);

	texcoords = bound(texcoords + time * velocity);

	return texture(image_texture, texcoords) * weight;
}

vec2 bound(vec2 coord)
{
	if (coord.x < 0)
		coord.x += 1;
	else if (coord.x > 1)
		coord.x -= 1;

	if (coord.y < 0)
		coord.y += 1;
	else if (coord.y > 1)
		coord.y -= 1;

	return coord;
}