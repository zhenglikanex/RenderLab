#pragma once
#include "Framework/Geometries/Geometry.hpp"

namespace Aurora {
	class Plane : public Geometry
	{
	public:
		Plane() : Geometry(GeometryType::kPlane) {};
		Plane(glm::vec3 normal, float intercept) : Geometry(GeometryType::kPlane), m_vNormal(normal), m_fIntercept(intercept) {};

		virtual void GetAabb(const glm::mat4& trans,
			glm::vec3& aabbMin,
			glm::vec3& aabbMax) const;

		glm::vec3 GetNormal() const { return m_vNormal; };
		float    GetIntercept() const { return m_fIntercept; };

	protected:
		glm::vec3 m_vNormal;
		float    m_fIntercept;
	};
}