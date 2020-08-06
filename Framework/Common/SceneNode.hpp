#pragma once

#include <memory>
#include <list>
#include <string>

#include "Framework/Common/SceneObject.hpp"
#include "Framework/Common/GeomMath.hpp"

namespace Aurora
{
	class BaseSceneNode
	{
	public:
		BaseSceneNode() {}
		BaseSceneNode(const std::string& name) : name_(name) {}
		virtual ~BaseSceneNode() {}

		void AppendChild(std::shared_ptr<BaseSceneNode>&& sub_node)
		{
			children_.push_back(std::move(sub_node));
		}

		void AppendChild(std::shared_ptr<SceneObjectTransform>&& transform)
		{
			transforms_.push_back(std::move(transform));
		}

		const std::shared_ptr<glm::mat4> GetCalculatedTransform() const
		{
			std::shared_ptr<glm::mat4> result = std::make_shared<glm::mat4>(glm::identity<glm::mat4>());
			
			// TODO:cascading calculation
			for (auto& trans : transforms_)
			{
				*result = *result * static_cast<glm::mat4>(*trans);
			}

			return result;
		}

	protected:
		virtual void dump(std::ostream& out) const {}
	protected:
		std::string name_;
		std::list<std::shared_ptr<BaseSceneNode>> children_;
		std::list<std::shared_ptr<SceneObjectTransform>> transforms_;
	};

	template<typename T>
	class SceneNode : public BaseSceneNode
	{
	public:
		using BaseSceneNode::BaseSceneNode;
		SceneNode() = default;
		SceneNode(const std::shared_ptr<T>& object) : scene_object_(object) {}
		SceneNode(std::shared_ptr<T>&& object) : scene_object_(std::move(object)) {}

		void AddSceneObjectRef(const std::string& object) { scene_object_ = object; }
		const std::string& GetSceneObjectRef() { return scene_object_; }
	protected:
		std::string scene_object_;
	};

	using SceneEmptyNode = BaseSceneNode;

	class SceneGeometryNode : public SceneNode<SceneObjectGeometry>
	{
	public:
		using SceneNode::SceneNode;

		void SetVisibility(bool visible) { visible_ = visible; }
		bool IsVisible() const { return visible_; }
		void SetIfCastShadow(bool shadow) { shadow_ = shadow; }
		bool IsCastShadow() const { return shadow_; }
		void SetIfMotionBlur(bool motion_blur) { motion_blur_ = motion_blur; }
		bool IsMotionBlur() const { return motion_blur_; }

		using SceneNode::AddSceneObjectRef;
		void AddMaterialRef(const std::string& material) { materials_.push_back(material); }
	protected:
		bool visible_;
		bool shadow_;
		bool motion_blur_;
		std::vector<std::string> materials_;
	};

	class SceneLightNode : public SceneNode<SceneObjectLight>
	{
	public:
		using SceneNode::SceneNode;

		void SetIfCastShadow(bool shadow) { shadow_ = shadow; }
		bool IsCastShadow() const { return shadow_; }

		void SetTarget(const glm::vec3& target) { target_ = target; }
		const glm::vec3& GetTarget() const { return target_; }
	protected:
		bool shadow_;
		glm::vec3 target_;
	};

	class SceneCameraNode : public SceneNode<SceneObjectCamera>
	{
	public:
		using SceneNode::SceneNode;

		void SetTarget(const glm::vec3& target) { target_ = target; }
		const glm::vec3& GetTarget() const { return target_; }
	protected:
		glm::vec3 target_;
	};
}