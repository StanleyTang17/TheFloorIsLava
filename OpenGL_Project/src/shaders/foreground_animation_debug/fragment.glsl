// IN

in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} fs_in;

in vec4 debug_color;
in float use_debug_color;


// OUT

out vec4 fs_color;



// UNIFORMS

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;


// MAIN

void main()
{
    if (use_debug_color > 0)
        fs_color = debug_color;
    else
	    fs_color = texture(texture_diffuse1, fs_in.texcoord);
}
