#pragma once

#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>

#include "Framework/Common/GeomMath.hpp"
#include "Framework/Common/Guid.hpp"
#include "Framework/Common/Image.hpp"
#include "Framework/Common/ImageParser.hpp"

using namespace xg;

namespace Aurora
{
	enum class SceneObjectType
	{
		kSceneObjectTypeMesh,
		kSceneObjectTypeMaterial,
		kSceneObjectTypeTexture,
		kSceneObjectTypeLight,
		kSceneObjectTypeCamera,
		kSceneObjectTypeAnimator,
		kSceneObjectTypeClip,
		kSceneObjectTypeVertexArray,
		kSceneObjectTypeIndexArray,
		kSceneObjectTypeGeometry,
		kSceneObjectTypeTransform,
		kSceneObjectTypeTranslate,
		kSceneObjectTypeRotate,
		kSceneObjectTypeScale
	};

	enum class SceneObjectCollisionType
	{
		kSceneObjectCollisionTypeNone,
		kSceneObjectCollisionTypeSphere,
		kSceneObjectCollisionTypeBox,
		kSceneObjectCollisionTypeCylinder,
		kSceneObjectCollisionTypeCapsule,
		kSceneObjectCollisionTypeCone,
		kSceneObjectCollisionTypeMultiSphere,
		kSceneObjectCollisionTypeConvexHull,
		kSceneObjectCollisionTypeConvexMesh,
		kSceneObjectCollisionTypeBvhMesh,
		kSceneObjectCollisionTypeHeightfield,
		kSceneObjectCollisionTypePlane

	};

	std::ostream& operator<<(std::ostream& out, SceneObjectType type);

	class SceneObjectTexture;

	template<typename T>
	struct ParameterValueMap
	{
		T Value;
		std::shared_ptr<SceneObjectTexture> ValueMap;

		ParameterValueMap() = default;
		ParameterValueMap(T value) : Value(value) {}
		ParameterValueMap(const std::shared_ptr<SceneObjectTexture>& value) : ValueMap(value) {}

		ParameterValueMap(const ParameterValueMap&) = default;
		ParameterValueMap(ParameterValueMap&&) = default;

		ParameterValueMap& operator=(const ParameterValueMap&) = default;
		ParameterValueMap& operator=(ParameterValueMap&&) = default;

		ParameterValueMap& operator=(const std::shared_ptr<SceneObjectTexture>& value)
		{
			ValueMap = value;
			return *this;
		}
	};

	using Color = ParameterValueMap<glm::vec4>;
	using Normal = ParameterValueMap<glm::vec3>;
	using Parameter = ParameterValueMap<float>;

}