#pragma once

#include "Framework/Common/BaseSceneObject.hpp"
#include "Framework/Common/SceneObjectMesh.hpp"

namespace Aurora
{
	class SceneObjectGeometry : public BaseSceneObject
	{
	public:
		SceneObjectGeometry() : BaseSceneObject(SceneObjectType::kSceneObjectTypeGeometry), collision_type_(SceneObjectCollisionType::kSceneObjectCollisionTypeNone) {}

		void SetVisibility(bool visible) { visible_ = visible; }
		bool IsVisible() const { return visible_; }
		void SetIfCastShadow(bool cast_shadow) { cast_shadow_ = cast_shadow; }
		bool IsCastVisible() const { return cast_shadow_; }
		void SetIfMotionBlur(bool motion_blur) { motion_blur_ = motion_blur; }
		bool IsMotionBlur() const { return motion_blur_; }
		void SetCollisionType(SceneObjectCollisionType collision_type) { collision_type_ = collision_type; }
		SceneObjectCollisionType CollisionType() const { return collision_type_; }
		void SetCollisionParameters(const float* param, int32_t count)
		{
			assert(count > 0 && count < 10);
			memcpy(collision_pamameters_, param, count * sizeof(float));
		}
		const float* GetCollisionParameters() const { return collision_pamameters_; }

		void AddMesh(const std::shared_ptr<SceneObjectMesh>& mesh) { meshs_.push_back(mesh); }
		void AddMesh(std::shared_ptr<SceneObjectMesh>&& mesh) { meshs_.push_back(std::move(mesh)); }
		const std::weak_ptr<SceneObjectMesh> GetMesh() { return meshs_.empty() ? nullptr : meshs_[0]; }
		const std::weak_ptr<SceneObjectMesh> GetMeshLOD(size_t lod) { return lod < meshs_.size() ? meshs_[lod] : nullptr; }
		glm::vec3 GetBoundingBox() const { return meshs_.empty() ? glm::vec3(0) : meshs_[0]->GetBoundingBox(); }
	protected:
		std::vector<std::shared_ptr<SceneObjectMesh>> meshs_;
		bool visible_;
		bool cast_shadow_;
		bool motion_blur_;
		SceneObjectCollisionType collision_type_;
		float collision_pamameters_[10];
	};
}