#include "Box.hpp"

using namespace Aurora;

void Box::GetAabb(const glm::mat4& trans, glm::vec3& aabbMin, glm::vec3& aabbMax) const
{
	TransformAabb(m_vDimension, m_fMargin, trans,
		aabbMin, aabbMax);
}
