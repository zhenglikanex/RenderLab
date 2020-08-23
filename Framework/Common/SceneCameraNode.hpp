#pragma once

#include "Framework/Common/BaseSceneNode.hpp"

namespace Aurora
{
	class SceneCameraNode : public SceneNode<SceneObjectCamera>
	{
	public:
		using SceneNode::SceneNode;

		void SetTarget(const glm::vec3& target) { target_ = target; }
		const glm::vec3& GetTarget() const { return target_; }

		glm::mat3 GetLocalAxis() override
		{
			auto transform = *GetCalculatedTransform();
			glm::vec3 target = GetTarget();
			glm::vec3 camera_position = glm::vec3(0.0f);
			camera_position = transform * glm::vec4(camera_position, 1.0f);
			glm::vec3 camera_z_axis = camera_position - target_;
			camera_z_axis = glm::normalize(camera_z_axis);
			glm::vec3 up(0.0f, 1.0f, 0.0f);
			glm::vec3 camera_x_axis = glm::cross(camera_z_axis, up);
			glm::vec3 camera_y_axis = glm::cross(camera_x_axis, camera_z_axis);
			glm::mat3 result(camera_x_axis, camera_y_axis, camera_z_axis);
			return result;
		}
	protected:
		glm::vec3 target_ = glm::vec3(0.0f);
	};
}