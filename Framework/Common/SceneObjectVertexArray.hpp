#pragma once

#include "BaseSceneObject.hpp"

namespace Aurora
{
	enum class VertexDataType
	{
		kVertexDataTypeFloat1,
		kVertexDataTypeFloat2,
		kVertexDataTypeFloat3,
		kVertexDataTypeFloat4,
		kVertexDataTypeDouble1,
		kVertexDataTypeDouble2,
		kVertexDataTypeDouble3,
		kVertexDataTypeDouble4,
	};

	// 將頂點已SOA的方式存儲,有利於GPU讀取
	class SceneObjectVertexArray : public BaseSceneObject
	{
	public:
		SceneObjectVertexArray(const std::string& attr = "", uint32_t morph_index = 0, VertexDataType data_type = VertexDataType::kVertexDataTypeFloat3, const void* data = nullptr, size_t data_size = 0)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeVertexArray)
			, attribute_(attr)
			, morph_target_index_(morph_index)
			, data_type_(data_type)
			, data_(data)
			, data_size_(data_size)
		{
		}

		SceneObjectVertexArray(const SceneObjectVertexArray&) = default;
		SceneObjectVertexArray(SceneObjectVertexArray&&) = default;

		SceneObjectVertexArray& operator=(SceneObjectVertexArray&) = default;
		SceneObjectVertexArray& operator=(SceneObjectVertexArray&&) = default;

		const std::string& GetAttributeName() const { return attribute_; }
		VertexDataType GetDataType() const { return data_type_; }
		size_t GetDataSize() const
		{
			size_t size = data_size_;

			switch (data_type_)
			{
			case VertexDataType::kVertexDataTypeFloat1:
			case VertexDataType::kVertexDataTypeFloat2:
			case VertexDataType::kVertexDataTypeFloat3:
			case VertexDataType::kVertexDataTypeFloat4:
				size *= sizeof(float);
				break;
			case VertexDataType::kVertexDataTypeDouble1:
			case VertexDataType::kVertexDataTypeDouble2:
			case VertexDataType::kVertexDataTypeDouble3:
			case VertexDataType::kVertexDataTypeDouble4:
				size *= sizeof(double);
				break;
			default:
				size = 0;
				assert(0);
				break;
			}

			return size;
		}
		const void* GetData() const { return data_; }
		size_t GetVertexCount() const
		{
			size_t size = data_size_;

			switch (data_type_) {
			case VertexDataType::kVertexDataTypeFloat1:
				size /= 1;
				break;
			case VertexDataType::kVertexDataTypeFloat2:
				size /= 2;
				break;
			case VertexDataType::kVertexDataTypeFloat3:
				size /= 3;
				break;
			case VertexDataType::kVertexDataTypeFloat4:
				size /= 4;
				break;
			case VertexDataType::kVertexDataTypeDouble1:
				size /= 1;
				break;
			case VertexDataType::kVertexDataTypeDouble2:
				size /= 2;
				break;
			case VertexDataType::kVertexDataTypeDouble3:
				size /= 3;
				break;
			case VertexDataType::kVertexDataTypeDouble4:
				size /= 4;
				break;
			default:
				size = 0;
				assert(0);
				break;
			}

			return size;
		}

	protected:
		std::string attribute_;
		uint32_t morph_target_index_;
		VertexDataType data_type_;

		const void* data_; // todo 不用釋放？
		size_t data_size_;
	};
}