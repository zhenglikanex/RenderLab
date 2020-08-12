#pragma once

#include "Framework/Common/GeomMath.hpp"

namespace Aurora
{
	enum class GeometryType
	{
		kBox,
		kSphere,
		kCylinder,
		kCone,
		kPlane,
		kCapsule,
		kTriangle
	};

	class Geometry
	{
	public:
		Geometry(GeometryType geometry_type);
		Geometry() = delete;
		virtual ~Geometry() = default;

		virtual void GetAabb(const glm::mat4& trans,
			glm::vec3& aabb_min,
			glm::vec3& aabb_max) const = 0;

		virtual void GetBoundingSphere(glm::vec3& center, float& radius) const;
		virtual float GetAngularMotionDisc() const;
		void CalculateTemporalAabb(const glm::mat4& curTrans,
			const glm::vec3& linvel,
			const glm::vec3& angvel,
			float timeStep,
			glm::vec3& temporalAabbMin,
			glm::vec3& temporalAabbMax) const;

		GeometryType GetGeometryType() const { return geometry_type_; };

	protected:
		GeometryType geometry_type_;
		float margin_;
	};
}