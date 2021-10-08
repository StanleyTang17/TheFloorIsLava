layout (location = 0) in vec3 vertex;

layout (binding = 0, std140) uniform Matrices
{
    mat4 projection_matrix;
    mat4 view_matrix;
    mat4 view_matrix_no_translate;
};

void main()
{
	gl_Position = projection_matrix * view_matrix * vec4(vertex, 1.0f);
}