#include "Scene.hpp"

using namespace Aurora;

const std::shared_ptr<SceneObjectCamera> Scene::GetCamera(const std::string& key) const
{
	auto iter = Cameras.find(key);
	if (iter == Cameras.end())
	{
		return nullptr;
	}
	return iter->second;
}

const std::shared_ptr<SceneCameraNode> Scene::GetFirstCameraNode() const
{
	return CameraNodes.empty() ? nullptr : CameraNodes.cbegin()->second;
}


const std::shared_ptr<SceneObjectLight> Scene::GetLight(const std::string& key) const
{
	auto iter = Lights.find(key);
	if (iter == Lights.end())
	{
		return nullptr;
	}
	return iter->second;
}
const std::shared_ptr<SceneLightNode> Scene::GetFirstLightNode() const
{
	return LightNodes.empty() ? nullptr : LightNodes.cbegin()->second;
}

const std::shared_ptr<SceneObjectMaterial> Scene::GetMaterial(const std::string& key) const
{
	auto iter = Materials.find(key);
	if (iter == Materials.end())
	{
		return nullptr;
	}
	return iter->second;
}

const std::shared_ptr<SceneObjectMaterial> Scene::GetFirstMaterial() const
{
	return Materials.empty() ? nullptr : Materials.cbegin()->second;
}

const std::shared_ptr<SceneObjectGeometry> Scene::GetGeometry(const std::string& key) const
{
	auto iter = Geometries.find(key);
	if (iter == Geometries.end())
	{
		return nullptr;
	}
	return iter->second;
}

const std::shared_ptr<SceneGeometryNode> Scene::GetFirstGeometryNode() const
{
	return GeometryNodes.empty() ? nullptr : GeometryNodes.cbegin()->second;
}

void Scene::LoadResource()
{
	for (auto& material : Materials)
	{
		material.second->LoadTextures();
	}
}