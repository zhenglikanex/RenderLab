#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef TWO_PI
#define TWO_PI 3.14159265358979323846f * 2.0f
#endif

#ifndef RAD
#define RAD 0.0174532925f
#endif

inline void TransformAabb(const glm::vec3& halfExtents, float margin, const glm::mat4& trans,
	glm::vec3& aabbMinOut, glm::vec3& aabbMaxOut)
{
	glm::vec3 halfExtentsWithMargin = halfExtents + glm::vec3(margin, margin, margin);
	glm::vec3  center;
	glm::vec3  extent;
	center = trans[3]; //get origin;

	extent[0] = glm::dot(trans[0], trans[0]);
	extent[1] = glm::dot(trans[1], trans[1]);
	extent[2] = glm::dot(trans[2], trans[2]);

	aabbMinOut = center - extent;
	aabbMaxOut = center + extent;
}