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

const std::shared_ptr<SceneCameraNode> Scene::GetNextCameraNode() const
{
	static thread_local auto _it = CameraNodes.cbegin();
	if (_it == CameraNodes.cend()) return nullptr;
	return (++_it == CameraNodes.cend()) ? nullptr : _it->second;
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

const std::shared_ptr<SceneLightNode> Scene::GetNextLightNode() const
{
	static thread_local auto _it = LightNodes.cbegin();
	if (_it == LightNodes.cend()) return nullptr;
	return (++_it == LightNodes.cend()) ? nullptr : _it->second;
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

const std::shared_ptr<SceneObjectMaterial> Scene::GetNextMaterial() const
{
	static thread_local auto _it = Materials.cbegin();
	if (_it == Materials.cend()) return _it->second;
	return (++_it == Materials.cend()) ? nullptr : _it->second;
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

const std::shared_ptr<SceneGeometryNode> Scene::GetNextGeometryNode() const
{
	static thread_local auto _it = GeometryNodes.cbegin();
	if (_it == GeometryNodes.cend()) return nullptr;
	return (++_it == GeometryNodes.cend()) ? nullptr : _it->second;
}

void Scene::LoadResource()
{
	for (auto& material : Materials)
	{
		material.second->LoadTextures();
	}
}