#pragma once

#include "Framework/Common/BaseSceneObject.hpp"

namespace Aurora
{
	enum class IndexDataType
	{
		kIndexDataTypeInt8,
		kIndexDataTypeInt16,
		kIndexDataTypeInt32,
		kIndexDataTypeInt64
	};

	class SceneObjectIndexArray :public BaseSceneObject
	{
	public:
		SceneObjectIndexArray(uint32_t material_index = 0, size_t restart_index = 0, IndexDataType data_type = IndexDataType::kIndexDataTypeInt16, const void* data = nullptr, size_t data_size = 0)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeIndexArray)
			, material_index_(material_index)
			, restart_index_(restart_index)
			, data_type_(data_type)
			, data_(data)
			, data_size_(data_size)
		{
		}

		SceneObjectIndexArray(const SceneObjectIndexArray&) = default;
		SceneObjectIndexArray(SceneObjectIndexArray&&) = default;

		SceneObjectIndexArray& operator=(SceneObjectIndexArray&) = default;
		SceneObjectIndexArray& operator=(SceneObjectIndexArray&&) = default;

		const uint32_t GetMaterialIndex() const { return material_index_; }
		const IndexDataType GetIndexType() const { return data_type_; }
		const void* GetData() const { return data_; }
		size_t GetDataSize() const
		{
			size_t size = data_size_;
			switch (data_type_)
			{
			case IndexDataType::kIndexDataTypeInt8:
				size *= sizeof(int8_t);
				break;
			case IndexDataType::kIndexDataTypeInt16:
				size *= sizeof(int16_t);
				break;
			case IndexDataType::kIndexDataTypeInt32:
				size *= sizeof(int32_t);
				break;
			case IndexDataType::kIndexDataTypeInt64:
				size *= sizeof(int64_t);
				break;
			default:
				size = 0;
				assert(0);
				break;
			}

			return size;
		}

		size_t GetIndexCount() const { return data_size_; }
	protected:
		uint32_t material_index_;
		size_t restart_index_;
		IndexDataType data_type_;

		const void* data_; // todo ≤ª”√·å∑≈£ø
		size_t data_size_;
	};
}