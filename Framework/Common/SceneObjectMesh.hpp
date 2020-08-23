#pragma once

#include "Framework/Common/SceneObjectVertexArray.hpp"
#include "Framework/Common/SceneObjectIndexArray.hpp"

namespace Aurora
{
	enum class PrimitiveType : int32_t {
		kPrimitiveTypeNone,        ///< No particular primitive type.
		kPrimitiveTypePointList,   ///< For N>=0, vertex N renders a point.
		kPrimitiveTypeLineList,    ///< For N>=0, vertices [N*2+0, N*2+1] render a line.
		kPrimitiveTypeLineStrip,   ///< For N>=0, vertices [N, N+1] render a line.
		kPrimitiveTypeTriList,     ///< For N>=0, vertices [N*3+0, N*3+1, N*3+2] render a triangle.
		kPrimitiveTypeTriFan,      ///< For N>=0, vertices [0, (N+1)%M, (N+2)%M] render a triangle, where M is the vertex count.
		kPrimitiveTypeTriStrip,    ///< For N>=0, vertices [N*2+0, N*2+1, N*2+2] and [N*2+2, N*2+1, N*2+3] render triangles.
		kPrimitiveTypePatch,       ///< Used for tessellation.
		kPrimitiveTypeLineListAdjacency,       ///< For N>=0, vertices [N*4..N*4+3] render a line from [1, 2]. Lines [0, 1] and [2, 3] are adjacent to the rendered line.
		kPrimitiveTypeLineStripAdjacency,      ///< For N>=0, vertices [N+1, N+2] render a line. Lines [N, N+1] and [N+2, N+3] are adjacent to the rendered line.
		kPrimitiveTypeTriListAdjacency,        ///< For N>=0, vertices [N*6..N*6+5] render a triangle from [0, 2, 4]. Triangles [0, 1, 2] [4, 2, 3] and [5, 0, 4] are adjacent to the rendered triangle.
		kPrimitiveTypeTriStripAdjacency,       ///< For N>=0, vertices [N*4..N*4+6] render a triangle from [0, 2, 4] and [4, 2, 6]. Odd vertices Nodd form adjacent triangles with indices min(Nodd+1,Nlast) and max(Nodd-3,Nfirst).
		kPrimitiveTypeRectList,    ///< For N>=0, vertices [N*3+0, N*3+1, N*3+2] render a screen-aligned rectangle. 0 is upper-left, 1 is upper-right, and 2 is the lower-left corner.
		kPrimitiveTypeLineLoop,    ///< Like <c>kPrimitiveTypeLineStrip</c>, but the first and last vertices also render a line.
		kPrimitiveTypeQuadList,    ///< For N>=0, vertices [N*4+0, N*4+1, N*4+2] and [N*4+0, N*4+2, N*4+3] render triangles.
		kPrimitiveTypeQuadStrip,   ///< For N>=0, vertices [N*2+0, N*2+1, N*2+3] and [N*2+0, N*2+3, N*2+2] render triangles.
		kPrimitiveTypePolygon,     ///< For N>=0, vertices [0, N+1, N+2] render a triangle.
	};

	class SceneObjectMesh : public BaseSceneObject
	{
	public:
		SceneObjectMesh()
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeMesh)
		{
		}
		SceneObjectMesh(SceneObjectMesh&& mesh)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeMesh)
			, index_array_(std::move(mesh.index_array_))
			, vertex_array_(std::move(mesh.vertex_array_))
			, primitive_type_(mesh.primitive_type_)
		{
		}

		void AddIndexArray(SceneObjectIndexArray&& array) { index_array_.push_back(std::move(array)); }
		void AddVertexArray(SceneObjectVertexArray&& array) { vertex_array_.push_back(std::move(array)); }
		void SetPrimitiveType(PrimitiveType type) { primitive_type_ = type; }

		//sub mesh 两种submesh的分发，1.从顶点就区分，2.同一套顶点，划分不同的顶点索引，这里使用的是第二种(第二种可以减少顶点数量buffer读取也更加友好,不需要切vao)
		size_t GetIndexGroupCount() const { return index_array_.size(); }
		size_t GetIndexCount(const size_t index) const { return index >= index_array_.size() ? 0 : index_array_[index].GetIndexCount(); }
		size_t GetVertexCount() const { return vertex_array_.empty() ? 0 : vertex_array_[0].GetVertexCount(); }
		size_t GetVertexPropertiesCount() const { return vertex_array_.size(); }
		const SceneObjectVertexArray& GetVertexPropertyArray(const size_t index) const { return vertex_array_[index]; }
		const SceneObjectIndexArray& GetIndexArray(const size_t index) const { return index_array_[index]; }
		const PrimitiveType& GetPrimitiveType() { return primitive_type_; }
		glm::vec3 GetBoundingBox() const;
	protected:
		std::vector<SceneObjectIndexArray> index_array_;
		std::vector<SceneObjectVertexArray> vertex_array_;
		PrimitiveType primitive_type_;
	};
}