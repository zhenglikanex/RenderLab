#include "Sphere.hpp"

using namespace Aurora;

void Sphere::GetAabb(const glm::mat4& trans, glm::vec3& aabbMin, glm::vec3& aabbMax) const
{
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	center = trans * glm::vec4(center,1.0f);
	glm::vec3 extent(margin_, margin_, margin_);
	aabbMin = center - extent;
	aabbMax = center + extent;
}