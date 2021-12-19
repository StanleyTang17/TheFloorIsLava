in vec2 vs_texcoord;

out vec4 fs_color;

uniform sampler2D screen_texture;
uniform sampler2D bloom_blur_texture;
uniform sampler2D depth_texture;
uniform int filter_mode;
uniform int show_depth;
uniform float exposure;

const float offset = 1.0 / 300.0;
const vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
);
const float gamma = 2.2;

vec3 gamma_to_linear(vec3 color)
{
    return pow(color, vec3(gamma));
}

vec3 linear_to_gamma(vec3 color)
{
    return pow(color, vec3(1.0/gamma));
}

void main()
{
	vec3 hdr_color = gamma_to_linear(texture(screen_texture, vs_texcoord).rgb);
    vec3 bloom_blur = gamma_to_linear(texture(bloom_blur_texture, vs_texcoord).rgb);

    // additive blending
    vec3 result = hdr_color + bloom_blur;

    // exposure tone mapping
	result = vec3(1.0) - exp(-result * exposure);

    // image processing
	if (filter_mode == 1) {
		result = vec3(1.0 - texture(screen_texture, vs_texcoord));
	} else if (filter_mode == 2) {
		float average = 0.2126 * result.r + 0.7152 * result.g + 0.0722 * result.b;
		result = vec3(average);
	} else if (filter_mode > 2) {
        float kernel[9];

        if (filter_mode == 3) {
            kernel = float[](
                -1, -1, -1,
                -1,  9, -1,
                -1, -1, -1
            );
        } else if (filter_mode == 4) {
            kernel = float[](
                1.0 / 16, 2.0 / 16, 1.0 / 16,
                2.0 / 16, 4.0 / 16, 2.0 / 16,
                1.0 / 16, 2.0 / 16, 1.0 / 16  
            );
        }
    
        vec3 sample_tex[9];
        for(int i = 0; i < 9; i++)
        {
            sample_tex[i] = vec3(texture(screen_texture, vs_texcoord.st + offsets[i]));
        }
        vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
            col += sample_tex[i] * kernel[i];
    
        result = vec3(col);
	}

    /*
    // depth debug
    if(show_depth == 1)
    {
        float depth_value = texture(depth_texture, vs_texcoord).r;
        result = vec3(depth_value);
    }
    */

    result = linear_to_gamma(result);

    if (show_depth == 1)
        fs_color = vec4(bloom_blur, 1.0);
    else
        fs_color = vec4(result, 1.0);
    
}