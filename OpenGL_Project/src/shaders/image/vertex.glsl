layout (location = 0) in vec4 vertex;
out vec2 texcoords;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	vec4 translated_pos = model * vec4(vertex.xy, 1.0, 1.0);
	gl_Position = projection * vec4(translated_pos.xy, 0.0, 1.0);
	texcoords = vertex.zw;
}