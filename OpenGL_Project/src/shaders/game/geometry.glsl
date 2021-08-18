in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} gs_in[];

out GS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} gs_out;

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float time;

void copy_output(int index);
vec4 explode(vec4 position, vec3 normal);
vec3 get_normal();

void main()
{
    vec3 normal = get_normal();

    //gl_Position = explode(gl_in[0].gl_Position, normal);
    gl_Position = gl_in[0].gl_Position;
    copy_output(0);
    EmitVertex();

    //gl_Position = explode(gl_in[1].gl_Position, normal);
    gl_Position = gl_in[1].gl_Position;
    copy_output(1);
    EmitVertex();

    //gl_Position = explode(gl_in[2].gl_Position, normal);
    gl_Position = gl_in[2].gl_Position;
    copy_output(2);
    EmitVertex();

    EndPrimitive();
}

void copy_output(int index)
{
    gs_out.position = gs_in[index].position;
    gs_out.normal = gs_in[index].normal;
    gs_out.texcoord = gs_in[index].texcoord;
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 5.0;
    vec3 translate = vec3(0.0, 1.0, 0.0) * ((sin(time * 2) + 1.0) / 2.0) * magnitude;
    return position + vec4(translate, 0.0);
}

vec3 get_normal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[0].gl_Position) - vec3(gl_in[2].gl_Position);
    return normalize(cross(a, b));
}