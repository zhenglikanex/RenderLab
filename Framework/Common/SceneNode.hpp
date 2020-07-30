#pragma once

#include <memory>
#include <list>
#include <string>

#include "Framework/Common/SceneObject.hpp"

namespace Aurora
{
	class BaseSceneNode
	{
	public:
		BaseSceneNode() {}
		BaseSceneNode(const std::string& name) : name_(name) {}
		virtual ~BaseSceneNode() {}

		void AppendChild(std::unique_ptr<BaseSceneNode>&& sub_node)
		{
			children_.push_back(std::move(sub_node));
		}

		void AppendChild(std::unique_ptr<SceneObjectTransform>&& transform)
		{
			transforms_.push_back(std::move(transform));
		}
	protected:
		virtual void dump(std::ostream& out) const {}
	protected:
		std::string name_;
		std::list<std::unique_ptr<BaseSceneNode>> children_;
		std::list<std::unique_ptr<SceneObjectTransform>> transforms_;
	};

	template<typename T>
	class SceneNode : public BaseSceneNode
	{
	public:
		using BaseSceneNode::BaseSceneNode;
		SceneNode() = default;
		SceneNode(const std::shared_ptr<T>& object) : scene_object_(object) {}
		SceneNode(std::shared_ptr<T>&& object) : scene_object_(std::move(object)) {}

		void AddSceneObjectRef(const std::shared_ptr<T>& object) { scene_object_ = object; }
		void AddSceneObjectRef(std::shared_ptr<T>&& object) { scene_object_ = std::move(object); }
	protected:
		std::shared_ptr<T> scene_object_;
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
		void AddSceneObjectRef(const std::shared_ptr<SceneObjectMaterial>& object) { materials_.push_back(object); }
	protected:
		bool visible_;
		bool shadow_;
		bool motion_blur_;
		std::vector<std::shared_ptr<SceneObjectMaterial>> materials_;
	};

	class SceneLightNode : public SceneNode<SceneObjectLight>
	{
	public:
		using SceneNode::SceneNode;

		void SetTarget(const glm::vec3& target) { target_ = target; }
		const glm::vec3& GetTarget() const { return target_; }
	protected:
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