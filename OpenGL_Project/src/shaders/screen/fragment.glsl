in vec2 vs_texcoord;

out vec4 fs_color;

uniform sampler2D screen_texture;
uniform sampler2D depth_texture;
uniform int filter_mode;
uniform int show_depth;

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

void main()
{
	fs_color = texture(screen_texture, vs_texcoord);

	if (filter_mode == 1) {
		fs_color = vec4(vec3(1.0 - texture(screen_texture, vs_texcoord)), 1.0);
	} else if (filter_mode == 2) {
		float average = 0.2126 * fs_color.r + 0.7152 * fs_color.g + 0.0722 * fs_color.b;
		fs_color = vec4(average, average, average, 1.0);
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
    
        fs_color = vec4(col, 1.0);
	}

    if(show_depth == 1)
    {
        float depth_value = texture(depth_texture, vs_texcoord).r;
        fs_color = vec4(vec3(depth_value), 1.0);
    }
}