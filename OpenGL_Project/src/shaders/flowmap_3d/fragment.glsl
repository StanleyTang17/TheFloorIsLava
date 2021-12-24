in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} fs_in;

uniform float time;
uniform sampler2D texture_diffuse1;
uniform sampler2D flowmap_texture;
uniform vec3 color_enhance;

layout (location = 0) out vec4 fs_color;
layout (location = 1) out vec4 fs_bright_color;

vec2 bound(vec2 coord);
vec4 calculate_color(vec2 texcoords, sampler2D tex, float time);

void main()
{
	vec4 color1 = calculate_color(fs_in.texcoord, texture_diffuse1, time / 10);
	vec4 color2 = calculate_color(fs_in.texcoord, texture_diffuse1, time / 10 + 0.5);
	fs_color = (color1 + color2) * vec4(color_enhance, 1.0);

	// output bright color
	vec3 grayscale_vector = vec3(0.2126, 0.7152, 0.0722);
	float brightness = dot(fs_color.rgb, grayscale_vector);
	if (brightness > 1.0)
		fs_bright_color = vec4(fs_color.rgb, 1.0);
	else
		fs_bright_color = vec4(0.0, 0.0, 0.0, 1.0);
}

vec4 calculate_color(vec2 texcoords, sampler2D tex, float time)
{
	time = fract(time);

	vec4 flowmap_color = texture(flowmap_texture, texcoords);
	vec2 velocity = vec2((flowmap_color.r - 0.5) * 2, (flowmap_color.g - 0.5) * 2);
	float weight = 1.0 - abs(1 - 2 * time);

	texcoords = bound(texcoords + time * velocity);

	return texture(tex, texcoords) * weight;
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