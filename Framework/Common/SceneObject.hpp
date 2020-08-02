#pragma once

#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>

#include "Framework/Common/GeomMath.hpp"
#include "Framework/Common/Guid.hpp"
#include "Framework/Common/Image.hpp"

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
		kSceneObjectTypeGeometry
	};

	std::ostream& operator<<(std::ostream& out, SceneObjectType type);

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
		BaseSceneObject(Guid& guid,SceneObjectType type) : guid_(guid),type_(type){}
		BaseSceneObject(Guid&& guid, SceneObjectType type) : guid_(std::move(guid)), type_(type) {}
		BaseSceneObject(BaseSceneObject&& obj) :guid_(std::move(obj.guid_)),type_(obj.type_) {  }
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
		SceneObjectIndexArray(uint32_t material_index = 0,size_t restart_index = 0,IndexDataType data_type = IndexDataType::kIndexDataTypeInt16,const void* data = nullptr,size_t data_size = 0)
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

		const void* data_; // todo 不用釋放？
		size_t data_size_;
	};

	enum class PrimitiveType : int32_t {
		kPrimitiveTypeNone ,        ///< No particular primitive type.
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
		SceneObjectMesh(bool visible = true,bool shadow = true,bool motion_blur = true)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeMesh)
			, visible_(visible)
			, shadow_(shadow)
			, motion_blur_(motion_blur)
		{
		}
		SceneObjectMesh(SceneObjectMesh&& mesh)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeMesh)
			, index_array_(std::move(mesh.index_array_))
			, vertex_array_(std::move(mesh.vertex_array_))
			, primitive_type_(mesh.primitive_type_)
			, visible_(mesh.visible_)
			, shadow_(mesh.shadow_)
			, motion_blur_(mesh.motion_blur_)
		{
		}

		void AddIndexArray(SceneObjectIndexArray&& array) { index_array_.push_back(std::move(array)); }
		void AddVertexArray(SceneObjectVertexArray&& array) { vertex_array_.push_back(std::move(array)); }
		void SetPrimitiveType(PrimitiveType type) { primitive_type_ = type; }

		size_t GetIndexCount() const { return index_array_.empty() ? 0 : index_array_[0].GetIndexCount(); }
		size_t GetVertexCount() const { return vertex_array_.empty() ? 0 : vertex_array_[0].GetVertexCount(); }
		size_t GetVertexPropertiesCount() const { return vertex_array_.size(); }
		const SceneObjectVertexArray& GetVertexPropertyArray(const size_t index) const { return vertex_array_[index]; }
		const SceneObjectIndexArray& GetIndexArray(const size_t index) const { return index_array_[index]; }
		const PrimitiveType& GetPrimitiveType() { return primitive_type_; }
	protected:
		std::vector<SceneObjectIndexArray> index_array_;
		std::vector<SceneObjectVertexArray> vertex_array_;
		PrimitiveType primitive_type_;

		bool visible_ = true;
		bool shadow_ = false;
		bool motion_blur_ = false;
	};

	class SceneObjectTexture : public BaseSceneObject
	{
	public:
		SceneObjectTexture() : BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture) {}
		SceneObjectTexture(const std::string& name) : BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture),tex_coord_index_(0), name_(name) {}
		SceneObjectTexture(uint32_t tex_coord_index,const std::string& name)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture)
			, tex_coord_index_(tex_coord_index)
			, name_(name) 
		{
		}
		SceneObjectTexture(uint32_t tex_coord_index,const std::shared_ptr<Image>& image)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture)
			, tex_coord_index_(tex_coord_index)
			, image_(image)
		{
		}
		SceneObjectTexture(uint32_t tex_coord_index,std::shared_ptr<Image>&& image)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture)
			, tex_coord_index_(tex_coord_index)
			, image_(std::move(image))
		{
		}
	protected:
		uint32_t tex_coord_index_;
		std::string name_;
		std::shared_ptr<Image> image_;
		std::vector<glm::mat4> transforms_;
	};

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

	class SceneObjectMaterial : public BaseSceneObject
	{
	public:
		SceneObjectMaterial(const std::string& name) : BaseSceneObject(SceneObjectType::kSceneObjectTypeMaterial) {}
		SceneObjectMaterial(
			const std::string& name = "",
			Color&& base_color = glm::vec4(1.0f),
			Parameter&& metallic = 0.0f,
			Parameter&& roughness = 0.0f,
			Normal&& normal = glm::vec3(0.0f, 0.0f, 1.0f),
			Parameter&& specular = 0.0f,
			Parameter&& ao = 0.0f)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeMaterial)
			, base_color_(base_color)
			, metallic_(metallic)
			, roughness_(roughness)
			, normal_(normal)
			, specular_(specular)
			, ambient_occlusion_(ao)
		{
		}
		void SetName(const std::string& name) {  }
		void SetColor(const std::string& attrib, const glm::vec4& color)
		{
			if (attrib == "diffuse")
			{
				base_color_ = color;
			}
		}

		void SetParam(std::string attrib, float param)
		{

		}

		void SetTexture(const std::string& attrib, const std::string& texture_name)
		{
			if (attrib == "diffuse")
			{
				base_color_ = std::make_shared<SceneObjectTexture>(texture_name);
			}
		}
		
		void SetTexture(const std::string& attrib, const std::shared_ptr<SceneObjectTexture>& texture)
		{
			if (attrib == "diffuse")
			{
				base_color_ = texture;
			}
		}
	protected:
		std::string name_;
		Color base_color_;
		Parameter metallic_;
		Parameter roughness_;
		Normal normal_;
		Parameter specular_;
		Parameter ambient_occlusion_;
	};

	class SceneObjectGeometry : public BaseSceneObject
	{
	public:
		SceneObjectGeometry() : BaseSceneObject(SceneObjectType::kSceneObjectTypeGeometry) {}
		
		void SetVisibility(bool visible) { visible_ = visible; }
		bool IsVisible() const { return visible_; }
		void SetIfCastShadow(bool cast_shadow) { cast_shadow_ = cast_shadow; }
		bool IsCastVisible() const { return cast_shadow_; }
		void SetIfMotionBlur(bool motion_blur) { motion_blur_ = motion_blur; }
		bool IsMotionBlur() const { return motion_blur_; }

		void AddMesh(const std::shared_ptr<SceneObjectMesh>& mesh) { meshs_.push_back(mesh); }
		void AddMesh(std::shared_ptr<SceneObjectMesh>&& mesh) { meshs_.push_back(std::move(mesh)); }
		const std::weak_ptr<SceneObjectMesh> GetMesh() { return meshs_.empty() ? nullptr : meshs_[0]; }
		const std::weak_ptr<SceneObjectMesh> GetMeshLOD(size_t lod) { return lod < meshs_.size() ? meshs_[lod] : nullptr; }
	protected:
		std::vector<std::shared_ptr<SceneObjectMesh>> meshs_;
		bool visible_;
		bool cast_shadow_;
		bool motion_blur_;
	};

	//输入为光源的强度和到光源的距离，输出为光照强度的函数的指针
	using AttenFunc = float(*)(float /* Intensity */, float /* Distance */);
	
	inline float DefaultAttenFunc(float intensity, float distance)
	{
		return intensity / (1 + distance);
	}

	class SceneObjectLight : public BaseSceneObject
	{
	protected:
		SceneObjectLight(
			Color&& color = glm::vec4(1.0f),
			float intensity = 10.0f,
			AttenFunc atten_func = DefaultAttenFunc,
			float near_clip = 1.0f,
			float far_clip = 100.0f,
			bool cast_shadows = false)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeLight)
			, light_color_(std::move(color))
			, intensity_(intensity)
			, light_attenuation_(atten_func)
			, near_clip_distance_(near_clip)
			, far_clip_distance_(far_clip)
			, cast_shadows_(cast_shadows)
		{
		}
	protected:
		Color light_color_;
		float intensity_;
		AttenFunc light_attenuation_;	//允許指定特殊的衰減的函數
		float near_clip_distance_;
		float far_clip_distance_;
		bool cast_shadows_;
	};

	class SceneObjectOmniLight : public SceneObjectLight
	{
	public:
		using SceneObjectLight::SceneObjectLight;
	};

	class SceneObjectSpotLight : public SceneObjectLight
	{
	public:
		using SceneObjectLight::SceneObjectLight;
	protected:
		float cone_angle_;
		float penumbra_angle_;
	};

	class SceneObjectCamera : public BaseSceneObject
	{
	public:
		SceneObjectCamera() : BaseSceneObject(SceneObjectType::kSceneObjectTypeCamera) {}
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
	protected:
		float fov_;
	};

	class SceneObjectTransform
	{
	public:
		SceneObjectTransform() : matrix_(glm::identity<glm::mat4>()), scene_object_only_(false) {}
		SceneObjectTransform(const glm::mat4& matrix, bool object_only = false) : matrix_(matrix), scene_object_only_(object_only) {}
	protected:
		glm::mat4 matrix_;
		bool scene_object_only_;
	};

	class SceneObjectTranslation : public SceneObjectTransform
	{
	public:
		SceneObjectTranslation(char axis, float amount)
		{
			switch (axis)
			{
			case 'x':
				matrix_ = glm::translate(matrix_, glm::vec3(amount, 0.0f, 0.0f));
				break;
			case 'y':
				matrix_ = glm::translate(matrix_, glm::vec3(0.0f,amount, 0.0f));
				break;
			case 'z':
				matrix_ = glm::translate(matrix_, glm::vec3(0.0f, 0.0f, amount));
				break;
			default:
				assert(0);
			}
		}

		SceneObjectTranslation(float x, float y, float z)
		{
			matrix_ = glm::translate(matrix_, glm::vec3(x, y, z));
		}
	};

	class SceneObjectRotation : public SceneObjectTransform
	{
	public:
		SceneObjectRotation(const char axis, const float theta)
		{
			switch (axis) {
			case 'x':
				matrix_ = glm::rotate(matrix_,theta,glm::vec3(1.0f,0.0f,0.0f));
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
		}

		SceneObjectRotation(glm::vec3& axis, const float theta)
		{
			glm::normalize(axis);
			matrix_ = glm::rotate(matrix_, theta, axis);
		}

		SceneObjectRotation(const glm::quat& quaternion)
		{
			matrix_ = glm::mat4_cast(quaternion);
		}
	};

	class SceneObjectScale : public SceneObjectTransform
	{
	public:
		SceneObjectScale(const char axis, const float amount)
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
		}

		SceneObjectScale(const float x, const float y, const float z)
		{
			matrix_ = glm::scale(matrix_, glm::vec3(x, y, z));
		}
	};
}