layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;

out vec2 vs_texcoord;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
	vs_texcoord = vertex_texcoord;
	gl_Position = vec4(vertex_position.x, vertex_position.y, 0.0, 1.0);
}