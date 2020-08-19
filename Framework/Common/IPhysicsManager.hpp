#pragma once

#include <vector>

#include "Framework/Common/IRuntimeModule.hpp"
#include "Framework/Common/SceneManager.hpp"

namespace Aurora
{
	class IPhysicsManager : public IRuntimeModule
	{
	public:
		virtual void CreateRigidBody(SceneGeometryNode& node, const SceneObjectGeometry& geometry) = 0;
		virtual void DeleteRigidBody(SceneGeometryNode& node) = 0;

		virtual int CreateRigidBodies() = 0;
		virtual void ClearRigidBodies() = 0;

		virtual glm::mat4 GetRigidBodyTransform(void* rigidbody) = 0;
		virtual void UpdateRigidBodyTransform(SceneGeometryNode& node) = 0;
		virtual void ApplyCentralForce(void* rigidboy, const glm::vec3& force) = 0;
	};
}