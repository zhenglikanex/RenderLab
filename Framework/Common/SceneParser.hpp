#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Framework/Common/SceneNode.hpp"
#include "Framework/Common/SceneObject.hpp"

namespace Aurora
{
	class Scene 
	{
	public:
		Scene(const std::string& scene_name)
			:SceneGraph(new BaseSceneNode(scene_name))
		{
		}
	public:
		std::unique_ptr<BaseSceneNode> SceneGraph;
		std::unordered_map<std::string, std::shared_ptr<SceneObjectCamera>> Cameras;
		std::unordered_map<std::string, std::shared_ptr<SceneObjectLight>> Lights;
		std::unordered_map<std::string, std::shared_ptr<SceneObjectMaterial>> Materials;
		std::unordered_map<std::string, std::shared_ptr<SceneObjectGeometry>> Geometries;

		const std::shared_ptr<SceneObjectCamera> GetCamera(const std::string& key) const;
		const std::shared_ptr<SceneObjectCamera> GetFirstCamera() const;
		const std::shared_ptr<SceneObjectCamera> GetNextCamera() const;

		const std::shared_ptr<SceneObjectLight> GetLight(const std::string& key) const;
		const std::shared_ptr<SceneObjectLight> GetFirstLight() const;
		const std::shared_ptr<SceneObjectLight> GetNextLight() const;

		const std::shared_ptr<SceneObjectMaterial> GetMaterial(const std::string& key) const;
		const std::shared_ptr<SceneObjectMaterial> GetFirstMaterial() const;
		const std::shared_ptr<SceneObjectMaterial> GetNextMaterial() const;

		const std::shared_ptr<SceneObjectGeometry> GetGeometry(const std::string& key) const;
		const std::shared_ptr<SceneObjectGeometry> GetFirstGeometry() const;
		const std::shared_ptr<SceneObjectGeometry> GetNextGeometry() const;
	};

	class SceneParser
	{
	public:
		virtual std::unique_ptr<BaseSceneNode> Parser(const std::string& buf) = 0;
	};
}