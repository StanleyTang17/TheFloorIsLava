layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_tangent;
layout (location = 4) in vec3 vertex_bitangent;
layout (location = 5) in mat4 model_matrix;
layout (location = 9) in float top_block_landing_time;
layout (location = 10) in float is_enabled;

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
    float top_block_time;
    float enabled;
} vs_out;

void main()
{
    vs_out.position = vec3(model_matrix * vec4(vertex_position, 1.0f));
    vs_out.normal = mat3(transpose(inverse(model_matrix))) * vertex_normal;
    vs_out.texcoord = vertex_texcoord;
    vs_out.top_block_time = top_block_landing_time;
    vs_out.enabled = is_enabled;

    gl_Position = projection_matrix * view_matrix * vec4(vs_out.position, 1.0f);
}