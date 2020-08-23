#pragma once

#include "Framework/Common/BaseSceneObject.hpp"

namespace Aurora
{
	class SceneObjectCamera : public BaseSceneObject
	{
	public:
		SceneObjectCamera() : BaseSceneObject(SceneObjectType::kSceneObjectTypeCamera), aspect_(16.0f / 9.0f), near_clip_distance_(1.0f), far_clip_distance_(100.f) {}

		void SetColor(const std::string& attrib, const glm::vec4& color)
		{

		}

		void SetParam(const std::string& attrib, float param)
		{
			if (attrib == "near")
			{
				near_clip_distance_ = param;
			}
			else if (attrib == "far")
			{
				far_clip_distance_ = param;
			}
		}

		void SetTexture(const std::string& attrib, const std::string& texture_name)
		{

		}

		float GetNearClipDistance() { return near_clip_distance_; }
		float GetFarClipDistance() { return far_clip_distance_; }
	protected:
		float aspect_;
		float near_clip_distance_;
		float far_clip_distance_;
	};

	class SceneObjectOrthogonalCamera : public SceneObjectCamera
	{

	};

	class SceneObjectPerspectiveCamera : public SceneObjectCamera
	{
	public:
		SceneObjectPerspectiveCamera(float fov = PI / 2.0) : SceneObjectCamera(), fov_(fov) {}

		void SetParam(const std::string& attrib, float param)
		{
			if (attrib == "fov")
			{
				fov_ = param;
			}

			SceneObjectCamera::SetParam(attrib, param);
		}

		float GetFov() const { return fov_; }
	protected:
		float fov_;
	};
}