in vec2 vs_texcoord;

out vec4 fs_color;

uniform sampler2D screen_texture;
uniform sampler2D bloom_blur_texture;
uniform sampler2D depth_texture;
uniform int filter_mode;
uniform float exposure;
uniform vec2 inverse_screen_size;
uniform float fxaa_max_range;
uniform float fxaa_reduce_min;
uniform float fxaa_reduce_multiplier;

const float gamma = 2.2;

const vec3 luma = vec3(0.2126, 0.7152, 0.0722);

vec3 gamma_to_linear(vec3 color);
vec3 linear_to_gamma(vec3 color);
vec3 linear_hdr_tex(vec2 texcoord);
float luminosity(ivec2 texcoord_offset);

void main()
{
	vec3 hdr_color = gamma_to_linear(texture(screen_texture, vs_texcoord).rgb);
    vec3 bloom_blur = gamma_to_linear(texture(bloom_blur_texture, vs_texcoord).rgb);

    vec3 result = hdr_color;

    // filters
	if (filter_mode == 1) {
		result = vec3(1.0 - texture(screen_texture, vs_texcoord));
	} else if (filter_mode == 2) {
        vec3 grayscale_transform = vec3(0.2126, 0.7152, 0.0722);
		float average = dot(result.rgb, grayscale_transform);
		result = vec3(average);
	} else if (filter_mode == 3) {
        float lumaTL = luminosity(ivec2(-1, -1));
        float lumaTR = luminosity(ivec2( 1, -1));
        float lumaMM = luminosity(ivec2( 0,  0));
        float lumaBL = luminosity(ivec2(-1,  1));
        float lumaBR = luminosity(ivec2( 1,  1));

        vec2 dir = vec2(0.0); // blur direction
        dir.x = (lumaTR + lumaBR) - (lumaTL + lumaBL);
        dir.y = (lumaBL + lumaBR) - (lumaTL + lumaTR);

        float dir_reduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * 0.25 * fxaa_reduce_multiplier, fxaa_reduce_min);
        float scale = 1.0/(min(abs(dir.x), abs(dir.y)) - dir_reduce);

        dir *= scale;                                                       // scale either x or y to 1.0
        dir = clamp(dir, vec2(-fxaa_max_range), vec2(fxaa_max_range));      // limit sample range
        dir *= inverse_screen_size;                                         // scale to texcoord

        // smaller range blur
        vec3 blur_result_1 = 0.5 * (
            linear_hdr_tex(vs_texcoord + dir * (1.0/3.0 - 0.5)) +
            linear_hdr_tex(vs_texcoord + dir * (2.0/3.0 - 0.5))
        );

        // wider range blur
        vec3 blur_result_2 = 0.5 * blur_result_1 + 0.25 * (
            linear_hdr_tex(vs_texcoord + dir * (0.0/3.0 - 0.5)) +
            linear_hdr_tex(vs_texcoord + dir * (3.0/3.0 - 0.5))
        );

        float luma_min = min(lumaMM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
        float luma_max = max(lumaMM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
        float luma_result_2 = dot(blur_result_2, luma);

        if (luma_result_2 < luma_min || luma_result_2 > luma_max)
            result = blur_result_1;
        else
            result = blur_result_2;
	}

    // apply bloom effect
    result = result + bloom_blur;
	result = vec3(1.0) - exp(-result * exposure); // exposure tone mapping

    result = linear_to_gamma(result);

    fs_color = vec4(result, 1.0);
}

vec3 gamma_to_linear(vec3 color)
{
    return pow(color, vec3(gamma));
}

vec3 linear_to_gamma(vec3 color)
{
    return pow(color, vec3(1.0/gamma));
}

vec3 linear_hdr_tex(vec2 texcoord)
{
    return gamma_to_linear(texture(screen_texture, texcoord).rgb);
}

float luminosity(ivec2 texcoord_offset)
{
    vec2 texcoord = vs_texcoord + texcoord_offset * inverse_screen_size;
    vec3 color = linear_hdr_tex(texcoord);
    return dot(luma, color);
}