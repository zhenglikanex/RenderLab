#pragma once

#include "Framework/Common/IPhysicsManager.hpp"
#include "Framework/Geometries/Geometry.hpp"

namespace Aurora
{
	class MyPhysicsManager : public IPhysicsManager
	{
	public:
		bool Initialize() override;
		void Finalize() override;
		void Tick() override;

#ifdef DEBUG
		void DrawDebugInfo() override;
		void DrawAabb(const Geometry& geometry, const glm::mat4& trans);
#endif

		void CreateRigidBody(SceneGeometryNode& node, const SceneObjectGeometry& geometry) override;
		void DeleteRigidBody(SceneGeometryNode& node) override;

		int CreateRigidBodies() override;
		void ClearRigidBodies() override;

		glm::mat4 GetRigidBodyTransform(void* rigidbody) override;
		void UpdateRigidBodyTransform(SceneGeometryNode& node);
		void ApplyCentralForce(void* rigidboy, const glm::vec3& force) override;

	protected:
		std::vector<Geometry*> collision_shapes_;
	};
}