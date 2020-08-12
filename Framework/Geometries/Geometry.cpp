#include "Geometry.hpp"

using namespace Aurora;

void Geometry::CalculateTemporalAabb(
	const glm::mat4& curTrans,
	const glm::vec3& linvel,
	const glm::vec3& angvel,
	float timeStep,
	glm::vec3& temporalAabbMin,
	glm::vec3& temporalAabbMax) const
{
	GetAabb(curTrans, temporalAabbMin, temporalAabbMax);

	float temporalAabbMaxx = temporalAabbMax.x;
	float temporalAabbMaxy = temporalAabbMax.y;
	float temporalAabbMaxz = temporalAabbMax.z;
	float temporalAabbMinx = temporalAabbMin.x;
	float temporalAabbMiny = temporalAabbMin.y;
	float temporalAabbMinz = temporalAabbMin.z;

	// add linear motion
	glm::vec3 linMotion = linvel * timeStep;
	///@todo: simd would have a vector max/min operation, instead of per-element access
	if (linMotion.x > 0.0f)
		temporalAabbMaxx += linMotion.x;
	else
		temporalAabbMinx += linMotion.x;
	if (linMotion.y > 0.0f)
		temporalAabbMaxy += linMotion.y;
	else
		temporalAabbMiny += linMotion.y;
	if (linMotion.z > 0.0f)
		temporalAabbMaxz += linMotion.z;
	else
		temporalAabbMinz += linMotion.z;

	//add conservative angular motion
	float angularMotion = glm::length(angvel) * GetAngularMotionDisc() * timeStep;
	glm::vec3 angularMotion3d(angularMotion, angularMotion, angularMotion);
	temporalAabbMin = glm::vec3(temporalAabbMinx, temporalAabbMiny, temporalAabbMinz);
	temporalAabbMax = glm::vec3(temporalAabbMaxx, temporalAabbMaxy, temporalAabbMaxz);

	temporalAabbMin = temporalAabbMin - angularMotion3d;
	temporalAabbMax = temporalAabbMax + angularMotion3d;
}

void Geometry::GetBoundingSphere(glm::vec3& center, float& radius) const
{
	glm::mat4 tran = glm::identity<glm::mat4>();
	glm::vec3 aabbMin, aabbMax;
	GetAabb(tran, aabbMin, aabbMax);
	radius = glm::length(aabbMax - aabbMin) * 0.5f;
	center = (aabbMin + aabbMax) * 0.5f;
}

float Geometry::GetAngularMotionDisc() const
{
	glm::vec3 center;
	float disc = 0.0f;
	GetBoundingSphere(center, disc);
	disc += glm::length(center);
	return disc;
}