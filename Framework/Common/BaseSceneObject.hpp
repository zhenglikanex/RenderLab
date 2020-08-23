#pragma once

#include "Framework/Common/SceneObjectTypeDef.hpp"

namespace Aurora
{
	class BaseSceneObject
	{
	public:
		virtual ~BaseSceneObject() = default;

		const Guid& guid() const { return guid_; }

		friend std::ostream& operator<<(std::ostream& out, const BaseSceneObject& obj)
		{
			out << "SceneObject" << std::endl;
			out << "-----------" << std::endl;
			out << "GUID: " << obj.guid_ << std::endl;
			out << "Type: " << obj.type_ << std::endl;

			return out;
		}
	protected:
		BaseSceneObject(SceneObjectType type) : type_(type) { guid_ = newGuid(); }
		BaseSceneObject(Guid& guid, SceneObjectType type) : guid_(guid), type_(type) {}
		BaseSceneObject(Guid&& guid, SceneObjectType type) : guid_(std::move(guid)), type_(type) {}
		BaseSceneObject(BaseSceneObject&& obj) :guid_(std::move(obj.guid_)), type_(obj.type_) {  }
		BaseSceneObject& operator=(BaseSceneObject&& obj)
		{
			guid_ = std::move(obj.guid_);
			type_ = obj.type_;
		}
	private:
		BaseSceneObject() = delete;
		BaseSceneObject(const BaseSceneObject&) = delete;
		BaseSceneObject& operator=(const BaseSceneObject&) = delete;
	protected:
		Guid guid_;
		SceneObjectType type_;
	};
}