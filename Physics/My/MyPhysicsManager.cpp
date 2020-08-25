#include "MyPhysicsManager.hpp"

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Geometries/Box.hpp"
#include "Framework/Geometries/Plane.hpp"
#include "Framework/Geometries/Sphere.hpp"
#include "Physics/My/RigidBody.hpp"
#include "Physics/My/MotionState.hpp"

using namespace Aurora;

bool MyPhysicsManager::Initialize()
{
	return true;
}

void MyPhysicsManager::Finalize()
{
	ClearRigidBodies();
}

void MyPhysicsManager::Tick()
{
	if (g_app->GetEngine()->GetSceneManager()->IsSceneChanged())
	{
		ClearRigidBodies();
		CreateRigidBodies();
		g_app->GetEngine()->GetSceneManager()->NotifySceneIsPhysicsSimulationQueued();
	}

	DrawDebugInfo();
}

void MyPhysicsManager::CreateRigidBody(SceneGeometryNode& node, const SceneObjectGeometry& geometry)
{
	const float* param = geometry.GetCollisionParameters();
	RigidBody* rigidbody = nullptr;

	switch (geometry.CollisionType())
	{
	case SceneObjectCollisionType::kSceneObjectCollisionTypeSphere:
	{
		auto collision_box = std::make_shared<Sphere>(param[0]);
		const auto trans = node.GetCalculatedTransform();

		auto motion_state = std::make_shared<MotionState>(*trans);
		rigidbody = new RigidBody(collision_box, motion_state);
	}break;
	case SceneObjectCollisionType::kSceneObjectCollisionTypeBox:
	{
		auto collision_box = std::make_shared<Box>(glm::vec3(param[0], param[1], param[2]));

		const auto trans = node.GetCalculatedTransform();
		auto motionState =
			std::make_shared<MotionState>(
				*trans
				);
		rigidbody = new RigidBody(collision_box, motionState);
	}break;
	case SceneObjectCollisionType::kSceneObjectCollisionTypePlane:
	{
		auto collision_box = std::make_shared<Plane>(glm::vec3(param[0], param[1], param[2]), param[3]);

		const auto trans = node.GetCalculatedTransform();
		auto motionState =
			std::make_shared<MotionState>(
				*trans
				);
		rigidbody = new RigidBody(collision_box, motionState);
	}break;
	default:
	{
		// create AABB box according to Bounding Box 
		auto collision_box = std::make_shared<Box>(geometry.GetBoundingBox());

		const auto trans = node.GetCalculatedTransform();
		auto motionState =
			std::make_shared<MotionState>(
				*trans
				);
		rigidbody = new RigidBody(collision_box, motionState);
	}
	}

	node.LinkRigidBody(rigidbody);
}

void MyPhysicsManager::DeleteRigidBody(SceneGeometryNode& node)
{
	RigidBody* rigidbody = reinterpret_cast<RigidBody*>(node.UnlinkRigidBody());
	if (rigidbody)
	{
		delete rigidbody;
	}
}

int MyPhysicsManager::CreateRigidBodies()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
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

void MyPhysicsManager::ClearRigidBodies()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForPhysicalSimulation();
	// Geometries
	for (auto _it : scene.GeometryNodes)
	{
		auto pGeometryNode = _it.second;
		DeleteRigidBody(*pGeometryNode);
	}
}

glm::mat4 MyPhysicsManager::GetRigidBodyTransform(void* rigidbody)
{
	RigidBody* _rigidBody = reinterpret_cast<RigidBody*>(rigidbody);
	auto motionState = _rigidBody->GetMotionState();
	glm::mat4 trans = motionState->GetTransition();
	
	return trans;
}

void MyPhysicsManager::UpdateRigidBodyTransform(SceneGeometryNode& node)
{
	const auto trans = node.GetCalculatedTransform();
	auto rigidBody = node.RigidBody();
	auto motionState = reinterpret_cast<RigidBody*>(rigidBody)->GetMotionState();
	motionState->SetTransition(*trans);
}

void MyPhysicsManager::ApplyCentralForce(void* rigidboy, const glm::vec3& force)
{

}

#ifdef DEBUG
void MyPhysicsManager::DrawDebugInfo()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForPhysicalSimulation();

	// Geometries
	for (auto _it : scene.GeometryNodes)
	{
		auto pGeometryNode = _it.second;
		if (void* rigidBody = pGeometryNode->RigidBody()) {
			RigidBody* _rigidBody = reinterpret_cast<RigidBody*>(rigidBody);
			glm::mat4 simulated_result = GetRigidBodyTransform(_rigidBody);
			auto pGeometry = _rigidBody->GetCollisionShape();
			//DrawAabb(*pGeometry, simulated_result);
		}
	}
}

void MyPhysicsManager::DrawAabb(const Geometry& geometry, const glm::mat4& trans)
{
	glm::vec3 bbMin, bbMax;
	glm::vec3 color(0.5f, 0.5f, 0.5f);

	geometry.GetAabb(trans, bbMin, bbMax);
	g_app->GetEngine()->GetGraphicsManager()->DrawBox(bbMin, bbMax, color);
}
#endif