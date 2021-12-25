layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_tangent;
layout (location = 4) in vec3 vertex_bitangent;

uniform mat4 model_matrix;

layout (binding = 0, std140) uniform Matrices
{
    mat4 projection_matrix;
    mat4 view_matrix;
    mat4 view_matrix_no_translate;
};

out VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
    mat3 TBN;
} vs_out;

void main()
{
    vs_out.position = vec3(model_matrix * vec4(vertex_position, 1.0f));
    vs_out.normal = mat3(transpose(inverse(model_matrix))) * vertex_normal;
    vs_out.texcoord = vertex_texcoord;

    vec3 T = normalize(vec3(model_matrix * vec4(vertex_tangent,     0.0)));
    vec3 B = normalize(vec3(model_matrix * vec4(vertex_bitangent,   0.0)));
    vec3 N = normalize(vec3(model_matrix * vec4(vertex_normal,      0.0)));

    vs_out.TBN = mat3(T, B, N);

    gl_Position = projection_matrix * view_matrix * vec4(vs_out.position, 1.0f);
}