#pragma once

#include "Framework/Common/BaseSceneNode.hpp"

namespace Aurora
{
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
}