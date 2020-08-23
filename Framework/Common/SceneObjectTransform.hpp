#pragma once

#include "Framework/Common/BaseSceneObject.hpp"

namespace Aurora
{
	class SceneObjectTransform : public BaseSceneObject
	{
	public:
		SceneObjectTransform() : BaseSceneObject(SceneObjectType::kSceneObjectTypeTransform), matrix_(glm::identity<glm::mat4>()), scene_object_only_(false) {}
		SceneObjectTransform(const glm::mat4& matrix, bool object_only = false) : BaseSceneObject(SceneObjectType::kSceneObjectTypeTransform), matrix_(matrix), scene_object_only_(object_only) {}

		operator glm::mat4() { return matrix_; }
		operator const glm::mat4() const { return matrix_; }
	protected:
		glm::mat4 matrix_;
		bool scene_object_only_;
	};

	class SceneObjectTranslation : public SceneObjectTransform
	{
	public:
		SceneObjectTranslation() { type_ = SceneObjectType::kSceneObjectTypeTranslate; matrix_ = glm::identity<glm::mat4>(); }
		SceneObjectTranslation(char axis, float amount, bool scene_object_only = false)\
			: SceneObjectTranslation()
		{
			switch (axis)
			{
			case 'x':
				matrix_ = glm::translate(matrix_, glm::vec3(amount, 0.0f, 0.0f));
				break;
			case 'y':
				matrix_ = glm::translate(matrix_, glm::vec3(0.0f, amount, 0.0f));
				break;
			case 'z':
				matrix_ = glm::translate(matrix_, glm::vec3(0.0f, 0.0f, amount));
				break;
			default:
				assert(0);
			}
			scene_object_only_ = scene_object_only;
		}

		SceneObjectTranslation(float x, float y, float z, bool object_only)
			:SceneObjectTranslation()
		{
			matrix_ = glm::translate(matrix_, glm::vec3(x, y, z));
			scene_object_only_ = object_only;
		}
	};

	class SceneObjectRotation : public SceneObjectTransform
	{
	public:
		SceneObjectRotation() { type_ = SceneObjectType::kSceneObjectTypeRotate; matrix_ = glm::identity<glm::mat4>(); }
		SceneObjectRotation(const char axis, const float theta, bool object_only = false)
			:SceneObjectRotation()
		{
			switch (axis) {
			case 'x':
				matrix_ = glm::rotate(matrix_, theta, glm::vec3(1.0f, 0.0f, 0.0f));
				break;
			case 'y':
				matrix_ = glm::rotate(matrix_, theta, glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 'z':
				matrix_ = glm::rotate(matrix_, theta, glm::vec3(0.0f, 0.0f, 1.0f));
				break;
			default:
				assert(0);
			}

			scene_object_only_ = object_only;
		}

		SceneObjectRotation(glm::vec3& axis, const float theta, bool object_only = false)
			:SceneObjectRotation()
		{
			glm::normalize(axis);
			matrix_ = glm::rotate(matrix_, theta, axis);
			scene_object_only_ = object_only;
		}

		SceneObjectRotation(const glm::quat& quaternion, bool object_only = false)
			:SceneObjectRotation()
		{
			matrix_ = glm::mat4_cast(quaternion);
			scene_object_only_ = object_only;
		}
	};

	class SceneObjectScale : public SceneObjectTransform
	{
	public:
		SceneObjectScale() { type_ = SceneObjectType::kSceneObjectTypeScale; matrix_ = glm::identity<glm::mat4>(); }
		SceneObjectScale(const char axis, const float amount, bool object_only = false)
			:SceneObjectScale()
		{
			switch (axis) {
			case 'x':
				matrix_ = glm::scale(matrix_, glm::vec3(amount, 0.0f, 0.0f));
				break;
			case 'y':
				matrix_ = glm::scale(matrix_, glm::vec3(0.0f, amount, 0.0f));
				break;
			case 'z':
				matrix_ = glm::scale(matrix_, glm::vec3(0.0f, 0.0f, amount));
				break;
			default:
				assert(0);
			}
			scene_object_only_ = object_only;
		}

		SceneObjectScale(const float x, const float y, const float z, bool object_only = false)
			:SceneObjectScale()
		{
			matrix_ = glm::scale(matrix_, glm::vec3(x, y, z));
			scene_object_only_ = object_only;
		}
	};
}