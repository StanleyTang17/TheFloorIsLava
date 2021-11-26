in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
    float top_block_time;
    float enabled;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D warning_texture;
uniform float current_time;

out vec4 fs_color;

void main()
{
    if (fs_in.enabled > 0)
    {
        vec4 block_color = texture(texture_diffuse1, fs_in.texcoord);
        vec4 warning_color = texture(warning_texture, fs_in.texcoord);

        if (fs_in.normal.y > 0.9f && fs_in.top_block_time > 0 && current_time < fs_in.top_block_time && warning_color.a > 0.001)
            fs_color = warning_color;
        else
            fs_color = block_color;
    }
    else
    {
        discard;
    }
}