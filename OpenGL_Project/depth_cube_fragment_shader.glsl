in vec4 position;

uniform vec3 light_pos;
uniform float far_plane;

void main()
{
	float distance = length(vec3(position) - light_pos);
	float light_distance = distance / far_plane;
	gl_FragDepth = light_distance;
}