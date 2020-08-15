#include "MyPhysicsManager.hpp"

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Geometries/Box.hpp"
#include "Framework/Geometries/Plane.hpp"
#include "Framework/Geometries/Sphere.hpp"

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
}

void MyPhysicsManager::CreateRigidBody(SceneGeometryNode& node, const SceneObjectGeometry& geometry)
{
	const float* param = geometry.GetCollisionParameters();
	Geometry* collision_box = nullptr;

	switch (geometry.CollisionType())
	{
	case SceneObjectCollisionType::kSceneObjectCollisionTypeSphere:
	{
		collision_box = new Sphere(param[0]);
		collision_shapes_.push_back(collision_box);
	}break;
	case SceneObjectCollisionType::kSceneObjectCollisionTypeBox:
	{
		collision_box = new Box(glm::vec3(param[0], param[1], param[2]));
		collision_shapes_.push_back(collision_box);
	}break;
	case SceneObjectCollisionType::kSceneObjectCollisionTypePlane:
	{
		collision_box = new Plane(glm::vec3(param[0], param[1], param[2]), param[3]);
		collision_shapes_.push_back(collision_box);
	}break;
	default:
		break;
	}

	node.LinkRigidBody(collision_box);
}

void MyPhysicsManager::DeleteRigidBody(SceneGeometryNode& node)
{
	Geometry* collision_box = reinterpret_cast<Geometry*>(node.UnlinkRigidBody());
	if (collision_box)
	{
		delete collision_box;
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
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
	// Geometries
	for (auto _it : scene.GeometryNodes)
	{
		auto pGeometryNode = _it.second;
		DeleteRigidBody(*pGeometryNode);
	}
	for (auto shape : collision_shapes_)
	{
		delete shape;
	}
	collision_shapes_.clear();
}

glm::mat4 MyPhysicsManager::GetRigidBodyTransform(void* rigidbody)
{
	glm::mat4 trans = glm::identity<glm::mat4>();
	return trans;
}

void MyPhysicsManager::ApplyCentralForce(void* rigidboy, const glm::vec3& force)
{

}

#ifdef DEBUG
void MyPhysicsManager::DrawDebugInfo()
{
	glm::vec3 from(-10.0f, 0.0f, 0.0f);
	glm::vec3 to(10.0f, 0.0f, 0.0f);
	glm::vec3 color(1.0f, 0.0f, 0.0f);
	g_app->GetEngine()->GetGraphicsManager()->DrawLine(from, to, color);
}
#endif