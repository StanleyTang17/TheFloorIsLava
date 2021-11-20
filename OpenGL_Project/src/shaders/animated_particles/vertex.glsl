layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in mat4 model_matrix;
layout (location = 7) in float texture_atlas_index;

layout (binding = 0, std140) uniform Matrices
{
    mat4 projection_matrix;
    mat4 view_matrix;
    mat4 view_matrix_no_translate;
};

out vec2 vs_texcoord;
out float atlas_index;

void main()
{
    vec3 position = vec3(model_matrix * vec4(vertex_position, 1.0f));
    gl_Position = projection_matrix * view_matrix * vec4(position, 1.0f);

    vs_texcoord = vertex_texcoord;
    atlas_index = texture_atlas_index;
}