layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_tangent;
layout (location = 4) in vec3 vertex_bitangent;
layout (location = 5) in mat4 model_matrix;

layout (binding = 0, std140) uniform Matrices
{
    mat4 projection_matrix;
    mat4 view_matrix;
    mat4 view_matrix_no_translate;
    mat4 light_space_matrix;
};

void main()
{
    gl_Position = light_space_matrix * model_matrix * vec4(vertex_position, 1.0);
}