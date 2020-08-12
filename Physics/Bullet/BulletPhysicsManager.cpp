#include "BulletPhysicsManager.hpp"

#include <iostream>

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"

using namespace Aurora;
using namespace std;

bool BulletPhysicsManager::Initialize()
{
	// Build the broadphase
	bt_broadphase_ = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	bt_collision_configuration_ = new btDefaultCollisionConfiguration();
	bt_dispatcher_ = new btCollisionDispatcher(bt_collision_configuration_);

	// The actual physics solver
	bt_solver_ = new btSequentialImpulseConstraintSolver;

	// The world
	bt_dynamics_world_ = new btDiscreteDynamicsWorld(bt_dispatcher_, bt_broadphase_, bt_solver_, bt_collision_configuration_);
	bt_dynamics_world_->setGravity(btVector3(0.0f, 0.0f, -9.8f));

	return true;
}

void BulletPhysicsManager::Finalize()
{
	// Clean up
	ClearRigidBodies();

	delete bt_dynamics_world_;
	delete bt_solver_;
	delete bt_dispatcher_;
	delete bt_collision_configuration_;
	delete bt_broadphase_;
}

void BulletPhysicsManager::Tick()
{
	if (g_app->GetEngine()->GetSceneManager()->IsSceneChanged())
	{
		this->ClearRigidBodies();
		this->CreateRigidBodies();

		g_app->GetEngine()->GetSceneManager()->NotifySceneIsPhysicsSimulationQueued();
	}

	bt_dynamics_world_->stepSimulation(1.0f / 60.0f, 10);
}

void BulletPhysicsManager::CreateRigidBody(SceneGeometryNode& node, const SceneObjectGeometry& geometry)
{
	btRigidBody* rigidBody = nullptr;
	auto param = geometry.GetCollisionParameters();
	switch (geometry.CollisionType())
	{
	case SceneObjectCollisionType::kSceneObjectCollisionTypeSphere:
	{
		btCollisionShape* sphere = new btSphereShape(param[0]);
		bt_collision_shapes_.push_back(sphere);

		const auto trans = *node.GetCalculatedTransform();
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(trans[3][0], trans[3][1], trans[3][2]));
		btDefaultMotionState* motionState =
			new btDefaultMotionState(
				startTransform
			);
		btScalar mass = 1.0f;
		btVector3 fallInertia(0.0f, 0.0f, 0.0f);
		sphere->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo
			rigidBodyCI(mass, motionState, sphere, fallInertia);
		rigidBody = new btRigidBody(rigidBodyCI);
		bt_dynamics_world_->addRigidBody(rigidBody);
	}
	break;
	case SceneObjectCollisionType::kSceneObjectCollisionTypeBox:
	{
		btCollisionShape* box = new btBoxShape(btVector3(param[0], param[1], param[2]));
		bt_collision_shapes_.push_back(box);

		const auto trans = *node.GetCalculatedTransform();
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(trans[3][0], trans[3][1], trans[3][2]));
		btDefaultMotionState* motionState =
			new btDefaultMotionState(
				btTransform(
					startTransform
				)
			);
		btRigidBody::btRigidBodyConstructionInfo
			rigidBodyCI(0.0f, motionState, box, btVector3(0.0f, 0.0f, 0.0f));
		rigidBody = new btRigidBody(rigidBodyCI);
		bt_dynamics_world_->addRigidBody(rigidBody);
	}
	break;
	case SceneObjectCollisionType::kSceneObjectCollisionTypePlane:
	{
		btCollisionShape* plane = new btStaticPlaneShape(btVector3(param[0],param[1],param[2]), param[3]);
		bt_collision_shapes_.push_back(plane);

		const auto trans = node.GetCalculatedTransform();
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3((*trans)[3].x, (*trans)[3].y, (*trans)[3].z));
		btDefaultMotionState* motionState =
			new btDefaultMotionState(
					startTransform
			);
		btRigidBody::btRigidBodyConstructionInfo
			rigidBodyCI(0.0f, motionState, plane, btVector3(0.0f, 0.0f, 0.0f));
		rigidBody = new btRigidBody(rigidBodyCI);
		bt_dynamics_world_->addRigidBody(rigidBody);
	}
	break;
	default:
		;
	}

	node.LinkRigidBody(rigidBody);
}

void BulletPhysicsManager::DeleteRigidBody(SceneGeometryNode& node)
{
	btRigidBody* rigidBody = reinterpret_cast<btRigidBody*>(node.UnlinkRigidBody());
	if (rigidBody) {
		bt_dynamics_world_->removeRigidBody(rigidBody);
		if (auto motionState = rigidBody->getMotionState())
			delete motionState;
		delete rigidBody;
		//m_btDynamicsWorld->removeCollisionObject(rigidBody);
	}
}

int BulletPhysicsManager::CreateRigidBodies()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForPhysicsSimulation();

	// Geometries
	for (auto _it : scene.GeometryNodes)
	{
		auto pGeometryNode = _it.second;
		auto pGeometry = scene.GetGeometry(pGeometryNode->GetSceneObjectRef());
		assert(pGeometry);

		CreateRigidBody(*pGeometryNode, *pGeometry);
	}

	return 0;
}

void BulletPhysicsManager::ClearRigidBodies()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();

	// Geometries
	for (auto _it : scene.GeometryNodes)
	{
		auto pGeometryNode = _it.second;
		DeleteRigidBody(*pGeometryNode);
	}

	for (auto shape : bt_collision_shapes_)
		delete shape;

	bt_collision_shapes_.clear();

}

glm::mat4 BulletPhysicsManager::GetRigidBodyTransform(void* rigidBody)
{
	glm::mat4 result = glm::identity<glm::mat4>();
	btTransform trans;
	reinterpret_cast<btRigidBody*>(rigidBody)->getMotionState()->getWorldTransform(trans);
	auto basis = trans.getBasis();
	auto origin = trans.getOrigin();
	
	result[0][0] = basis[0][0];
	result[1][0] = basis[0][1];
	result[2][0] = basis[0][2];
	result[0][1] = basis[1][0];
	result[1][1] = basis[1][1];
	result[2][1] = basis[1][2];
	result[0][2] = basis[2][0];
	result[1][2] = basis[2][1];
	result[2][2] = basis[2][2];
	result[3][0] = origin.getX();
	result[3][1] = origin.getY();
	result[3][2] = origin.getZ();

	return result;
}

void BulletPhysicsManager::ApplyCentralForce(void* rigidboy, const glm::vec3& force)
{
	btRigidBody* bt_rigidboy = reinterpret_cast<btRigidBody*>(rigidboy);
	btVector3 _force(force.x, force.y, force.z);	
	bt_rigidboy->activate(true);
	bt_rigidboy->applyCentralForce(_force);
}
