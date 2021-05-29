layout (location = 0) in vec3 vertex_position;

out vec3 vs_texcoord;

layout (binding = 0, std140) uniform Matrices
{
    mat4 projection_matrix;
    mat4 view_matrix;
    mat4 view_matrix_no_translate;
    mat4 light_space_matrix;
};

void main()
{
	gl_Position = projection_matrix * view_matrix_no_translate * vec4(vertex_position, 1.0);
	vs_texcoord = vertex_position;
}