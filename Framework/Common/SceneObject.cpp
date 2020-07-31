#include "SceneObject.hpp"

using namespace Aurora;

const std::string SceneObjectTypeOut[] =
{
	"kSceneObjectTypeMesh",
	"kSceneObjectTypeMaterial",
	"kSceneObjectTypeTexture",
	"kSceneObjectTypeLight",
	"kSceneObjectTypeCamera",
	"kSceneObjectTypeAnimator",
	"kSceneObjectTypeClip",
	"kSceneObjectTypeVertexArray",
	"kSceneObjectTypeIndexArray",
	"kSceneObjectTypeGeometry"
};

std::ostream& operator<<(std::ostream& out, SceneObjectType type)
{
	out << SceneObjectTypeOut[static_cast<uint32_t>(type)];
}
