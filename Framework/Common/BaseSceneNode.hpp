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
		BaseSceneNode(const std::string& name) :TreeNode(), name_(name) {}
		virtual ~BaseSceneNode() {}

		void PrependTransform(std::shared_ptr<SceneObjectTransform>&& transform)
		{
			transforms_.push_back(std::move(transform));
		}

		const std::shared_ptr<glm::mat4> GetCalculatedTransform() const
		{
			std::shared_ptr<glm::mat4> result = std::make_shared<glm::mat4>(glm::identity<glm::mat4>());

			// TODO:cascading calculation
			for (auto it = transforms_.rbegin(); it != transforms_.rend(); ++it)
			{
				auto trans = *it;
				*result = *result * static_cast<glm::mat4>(*trans);
			}

			// 将blender中的旋转抵消
			//*result *= glm::rotate(glm::identity<glm::mat4>(), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

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

		void MoveBy(float distance_x, float distance_y, float distance_z)
		{
			glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(distance_x, distance_y, distance_z));
			runtime_transform_ *= translation;
		}

		void MoveBy(const glm::vec3& distance)
		{
			MoveBy(distance.x, distance.y, distance.z);
		}

		virtual glm::mat3 GetLocalAxis()
		{
			return glm::identity<glm::mat3>();
		}
		const std::string& GetName() const { return name_; }
	protected:
		virtual void dump(std::ostream& out) const {}
	protected:
		std::string name_;
		std::list<std::shared_ptr<SceneObjectTransform>> transforms_;
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
}

