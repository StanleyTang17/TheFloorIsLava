#include"Math.h"

bool Math::find_POI(Line l, Plane p, glm::vec3& POI, float& t)
{
	if (glm::dot(l.L1, p.N) == 0)
		return false;

	float A = p.N.x, B = p.N.y, C = p.N.z, D = -glm::dot(l.L0, p.P0);
	float x = l.L0.x, y = l.L0.y, z = l.L0.z;
	
	t = -(glm::dot(p.N, l.L0) + -1 * glm::dot(p.N, p.P0)) / glm::dot(p.N, l.L1);
	POI = l.L0 + l.L1 * t;
	
	return true;
}