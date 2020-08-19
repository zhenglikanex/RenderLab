#include "SceneObject.hpp"

#include <numeric>

using namespace Aurora;

namespace Aurora 
{
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
		return out;
	}

	glm::vec3 SceneObjectMesh::GetBoundingBox() const
	{
		glm::vec3 bbmin(std::numeric_limits<float>::max());
		glm::vec3 bbmax(std::numeric_limits<float>::min());
		auto count = vertex_array_.size();
		for (auto n = 0; n < count; n++)
		{
			if (vertex_array_[n].GetAttributeName() == "position")
			{
				auto data_type = vertex_array_[n].GetDataType();
				auto vertices_count = vertex_array_[n].GetVertexCount();
				auto data = vertex_array_[n].GetData();
				for (auto i = 0; i < vertices_count; i++)
				{
					switch (data_type) {
					case VertexDataType::kVertexDataTypeFloat3:
					{
						const glm::vec3* vertex = reinterpret_cast<const glm::vec3*>(data) + i;
						bbmin.x = (bbmin.x < vertex->x) ? bbmin.x : vertex->x;
						bbmin.y = (bbmin.y < vertex->y) ? bbmin.y : vertex->y;
						bbmin.z = (bbmin.z < vertex->z) ? bbmin.z : vertex->z;
						bbmax.x = (bbmax.x > vertex->x) ? bbmax.x : vertex->x;
						bbmax.y = (bbmax.y > vertex->y) ? bbmax.y : vertex->y;
						bbmax.z = (bbmax.z > vertex->z) ? bbmax.z : vertex->z;
						break;
					}
					case VertexDataType::kVertexDataTypeDouble3:
					{
						const glm::dvec3* vertex = reinterpret_cast<const glm::dvec3*>(data) + i;
						bbmin.x = (bbmin.x < vertex->x) ? bbmin.x : vertex->x;
						bbmin.y = (bbmin.y < vertex->y) ? bbmin.y : vertex->y;
						bbmin.z = (bbmin.z < vertex->z) ? bbmin.z : vertex->z;
						bbmax.x = (bbmax.x > vertex->x) ? bbmax.x : vertex->x;
						bbmax.y = (bbmax.y > vertex->y) ? bbmax.y : vertex->y;
						bbmax.z = (bbmax.z > vertex->z) ? bbmax.z : vertex->z;
						break;
					}
					default:
						assert(0);
					}
				}
			}
		}

		return bbmax - bbmin;
	}
}
