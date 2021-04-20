layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_tangent;
layout (location = 4) in vec3 vertex_bitangent;

out vec3 vs_position;
out vec3 vs_normal;
out vec2 vs_texcoord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    vs_position = vec3(model_matrix * vec4(vertex_position, 1.0f));
    vs_normal = mat3(transpose(inverse(model_matrix))) * vertex_normal;
    vs_texcoord = vec2(vertex_texcoord.x, -vertex_texcoord.y);
    //vs_texcoord = vertex_texcoord;

    gl_Position = projection_matrix * view_matrix * vec4(vs_position, 1.0f);
}