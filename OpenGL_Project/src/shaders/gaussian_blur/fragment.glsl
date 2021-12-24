in vec2 vs_texcoord;
out vec4 color;

uniform sampler2D image_texture;
uniform bool horizontal;
const float weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 pixel_offset = 1.0 / textureSize(image_texture, 0);
	vec3 result = texture(image_texture, vs_texcoord).rgb * weights[0];

	for (int i = 1; i < 5; ++i)
	{
		float weight = weights[i];
		if (horizontal)
		{
			result += texture(image_texture, vs_texcoord + vec2(pixel_offset.x * i, 0.0)).rgb * weight;
			result += texture(image_texture, vs_texcoord - vec2(pixel_offset.x * i, 0.0)).rgb * weight;
		}
		else
		{
			result += texture(image_texture, vs_texcoord + vec2(0.0, pixel_offset.y * i)).rgb * weight;
			result += texture(image_texture, vs_texcoord - vec2(0.0, pixel_offset.y * i)).rgb * weight;
		}
	}

	color = vec4(result, 1.0);
}