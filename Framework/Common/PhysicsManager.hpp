#pragma once

#include <vector>

#include "Framework/Common/IRuntimeModule.hpp"
#include "Framework/Common/SceneManager.hpp"

#include "bullet/btBulletDynamicsCommon.h"

namespace Aurora
{
	class PhysicsManager : public IRuntimeModule
	{
	public:
		bool Initialize() override;
		void Finalize() override;
		void Tick() override;

		virtual void CreateRigidBody(SceneGeometryNode& node, const SceneObjectGeometry& geometry);
		virtual void DeleteRigidBody(SceneGeometryNode& node);
		
		virtual int CreateRigidBodies();
		virtual void ClearRigidBodies();

		glm::mat4 GetRigidBodyTransform(void* rigidbody);
		
	protected:
		btBroadphaseInterface* bt_broadphase_;
		btDefaultCollisionConfiguration* bt_collision_configuration_;
		btCollisionDispatcher* bt_dispatcher_;
		btSequentialImpulseConstraintSolver* bt_solver_;
		btDiscreteDynamicsWorld* bt_dynamics_world_;
		
		std::vector<btCollisionShape*> bt_collision_shapes_;
	};
}