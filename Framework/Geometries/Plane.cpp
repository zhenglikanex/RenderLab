#include "Plane.hpp"

using namespace Aurora;
using namespace std;

void Plane::GetAabb(const glm::mat4& trans, glm::vec3& aabbMin, glm::vec3& aabbMax) const
{
	(void)trans;
	float minf = numeric_limits<float>::min();
	float maxf = numeric_limits<float>::max();
	aabbMin = { minf, minf, minf };
	aabbMax = { maxf, maxf, maxf };
}