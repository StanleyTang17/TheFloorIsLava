layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_tangent;
layout (location = 4) in vec3 vertex_bitangent;
layout (location = 5) in vec4 bone_ids;
layout (location = 6) in vec4 weights;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 final_bone_matrices[MAX_BONES];

out VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} vs_out;

void main()
{
	vec4 final_position = vec4(0.0f);
    vec3 final_normal = vec3(0.0f);

    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        int index = int(bone_ids[i]);

        if (index == -1)
            continue;

        if (index >= MAX_BONES)
        {
            final_position = vec4(vertex_position, 1.0f);
            break;
        }

        mat4 bone_matrix = final_bone_matrices[index];

        final_position += bone_matrix * vec4(vertex_position, 1.0f) * weights[i];
        final_normal += mat3(bone_matrix) * vertex_normal * weights[i];
     }

    final_position = vec4(vec3(model_matrix * final_position), 1.0f);

    vs_out.position = final_position.xyz;
    vs_out.normal = final_normal;
    vs_out.texcoord = vertex_texcoord;

    gl_Position = projection_matrix * view_matrix * final_position;

}
