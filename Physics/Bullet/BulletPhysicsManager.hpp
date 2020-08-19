#pragma once

#include <vector>

#include "Framework/Common/IPhysicsManager.hpp"

#ifndef BT_USE_DOUBLE_PRECISION
#define BT_USE_DOUBLE_PRECISION 1
#endif

#include "bullet/btBulletDynamicsCommon.h"

namespace Aurora
{
	class BulletPhysicsManager : public IPhysicsManager
	{
	public:
		bool Initialize() override;
		void Finalize() override;
		void Tick() override;

		void CreateRigidBody(SceneGeometryNode& node, const SceneObjectGeometry& geometry) override;
		void DeleteRigidBody(SceneGeometryNode& node) override;
		
		int CreateRigidBodies() override;
		void ClearRigidBodies() override;

		glm::mat4 GetRigidBodyTransform(void* rigidbody) override;
		void UpdateRigidBodyTransform(SceneGeometryNode& node) override;
		void ApplyCentralForce(void* rigidboy, const glm::vec3& force) override;
	protected:
		btBroadphaseInterface* bt_broadphase_;
		btDefaultCollisionConfiguration* bt_collision_configuration_;
		btCollisionDispatcher* bt_dispatcher_;
		btSequentialImpulseConstraintSolver* bt_solver_;
		btDiscreteDynamicsWorld* bt_dynamics_world_;
		
		std::vector<btCollisionShape*> bt_collision_shapes_;
	};
}