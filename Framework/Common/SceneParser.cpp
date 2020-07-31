#include "SceneParser.hpp"

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

const std::shared_ptr<SceneObjectCamera> Scene::GetFirstCamera() const
{
	return Cameras.empty() ? nullptr : Cameras.cbegin()->second;
}

const std::shared_ptr<SceneObjectCamera> Scene::GetNextCamera() const
{
	static thread_local auto _it = Cameras.cbegin();
	if (_it == Cameras.cend()) return nullptr;
	return (++_it == Cameras.cend()) ? nullptr : _it->second;
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
const std::shared_ptr<SceneObjectLight> Scene::GetFirstLight() const
{
	return Lights.empty() ? nullptr : Lights.cbegin()->second;
}

const std::shared_ptr<SceneObjectLight> Scene::GetNextLight() const
{
	static thread_local auto _it = Lights.cbegin();
	if (_it == Lights.cend()) return nullptr;
	return (++_it == Lights.cend()) ? nullptr : _it->second;
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

const std::shared_ptr<SceneObjectGeometry> Scene::GetFirstGeometry() const
{
	return Geometries.empty() ? nullptr : Geometries.cbegin()->second;
}

const std::shared_ptr<SceneObjectGeometry> Scene::GetNextGeometry() const
{
	static thread_local auto _it = Geometries.cbegin();
	if (_it == Geometries.cend()) return nullptr;
	return (++_it == Geometries.cend()) ? nullptr : _it->second;
}