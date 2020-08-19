#pragma once

#include <memory>
#include <list>
#include <string>
#include <map>

#include "Framework/Common/SceneObject.hpp"
#include "Framework/Common/GeomMath.hpp"
#include "Framework/Algorism/Tree.hpp"

namespace Aurora
{
	class BaseSceneNode : public TreeNode
	{
	public:
		BaseSceneNode() :TreeNode() {}
		BaseSceneNode(const std::string& name) :TreeNode(),name_(name) {}
		virtual ~BaseSceneNode() {}

		void AppendTransform(const char* key,std::shared_ptr<SceneObjectTransform>&& transform)
		{
			transforms_.push_back(std::move(transform));
			LUTtransform_.insert({ std::string(key),transform });
		}

		std::shared_ptr<SceneObjectTransform> GetTransform(const std::string& key)
		{
			auto it = LUTtransform_.find(key);
			if (it != LUTtransform_.end())
			{
				return it->second;
			}
			else
			{
				return std::shared_ptr<SceneObjectTransform>();
			}
		}

		const std::shared_ptr<glm::mat4> GetCalculatedTransform() const
		{
			std::shared_ptr<glm::mat4> result = std::make_shared<glm::mat4>(glm::identity<glm::mat4>());
			
			// TODO:cascading calculation
			for (auto it = transforms_.rbegin();it != transforms_.rend();++it)
			{
				auto trans = *it;
				*result = *result * static_cast<glm::mat4>(*trans);
			}

			*result *= runtime_transform_;

			return result;
		}

		void RotateBy(float rotation_angle_x, float rotation_angle_y, float rotaion_angle_z)
		{
			glm::mat4 rotate = glm::identity<glm::mat4>();
			glm::mat4 rotate_x = glm::rotate(rotate, rotation_angle_x, glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 rotate_y = glm::rotate(rotate, rotation_angle_y, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 rotate_z = glm::rotate(rotate, rotaion_angle_z, glm::vec3(0.0f, 0.0f, 1.0f));

			runtime_transform_ = runtime_transform_ * rotate_x * rotate_y * rotate_z;
		}

		const std::string& GetName() const { return name_; }
	protected:
		virtual void dump(std::ostream& out) const {}
	protected:
		std::string name_;
		std::list<std::shared_ptr<SceneObjectTransform>> transforms_;
		std::map<std::string, std::shared_ptr<SceneObjectTransform>> LUTtransform_;
		glm::mat4 runtime_transform_ = glm::identity<glm::mat4>();
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
		std::string GetMaterialRef(const size_t index)
		{
			if (index < materials_.size())
			{
				return materials_[index];
			}
			return "";
		}

		void LinkRigidBody(void* rigidbody)
		{
			rigidbody_ = rigidbody;
		}

		void* UnlinkRigidBody()
		{
			void* rigidbody = rigidbody_;
			rigidbody_ = nullptr;

			return rigidbody;
		}

		void* RigidBody() const { return rigidbody_; }
	protected:
		bool visible_;
		bool shadow_;
		bool motion_blur_;
		std::vector<std::string> materials_;
		void* rigidbody_ = nullptr;
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