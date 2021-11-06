layout (location = 0) in vec3 vertex;

layout (binding = 0, std140) uniform Matrices
{
    mat4 projection_matrix;
    mat4 view_matrix;
    mat4 view_matrix_no_translate;
};

uniform mat4 model_matrix;

void main()
{
    vec3 position = vec3(model_matrix * vec4(vertex, 1.0f));
	gl_Position = projection_matrix * view_matrix * vec4(position, 1.0f);
}