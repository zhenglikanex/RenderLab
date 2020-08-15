#pragma once

#include "Framework/Geometries/Geometry.hpp"

namespace Aurora
{
	class Sphere : public Geometry
	{
	public:
		Sphere() : Geometry(GeometryType::kSphere) {};
		Sphere(const float radius) : Geometry(GeometryType::kSphere) {}

		virtual void GetAabb(const glm::mat4& trans,
			glm::vec3& aabbMin,
			glm::vec3& aabbMax) const;

		float GetRadius() const { return radius_; };

	protected:
		float radius_;
	};
}