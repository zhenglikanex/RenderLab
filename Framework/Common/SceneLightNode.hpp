#pragma once

#include "Framework/Common/BaseSceneNode.hpp"

namespace Aurora
{
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
}