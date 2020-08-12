#pragma once
#include "Framework/Geometries/Geometry.hpp"

namespace Aurora {
	class Box : public Geometry
	{
	public:
		Box() : Geometry(GeometryType::kBox) {};
		Box(glm::vec3 dimension) : Geometry(GeometryType::kBox), m_vDimension(dimension) {};

		virtual void GetAabb(const glm::mat4& trans,
			const glm::vec3& aabbMin,
			const glm::vec3& aabbMax) const;

		glm::vec3 GetDimension() const { return m_vDimension; };
		glm::vec3 GetDimensionWithMargin() const { return m_vDimension + margin_; };

	protected:
		glm::vec3 m_vDimension;
	};
}