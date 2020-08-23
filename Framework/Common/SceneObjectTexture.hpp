#pragma once

namespace Aurora
{
	class SceneObjectTexture : public BaseSceneObject
	{
	public:
		SceneObjectTexture() : BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture), tex_coord_index_(0) {}
		SceneObjectTexture(const std::string& name) : BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture), tex_coord_index_(0), name_(name) {}
		SceneObjectTexture(uint32_t tex_coord_index, const std::string& name)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture)
			, tex_coord_index_(tex_coord_index)
			, name_(name)
		{
		}
		SceneObjectTexture(uint32_t tex_coord_index, const std::shared_ptr<Image>& image)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture)
			, tex_coord_index_(tex_coord_index)
			, image_(image)
		{
		}
		SceneObjectTexture(uint32_t tex_coord_index, std::shared_ptr<Image>&& image)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture)
			, tex_coord_index_(tex_coord_index)
			, image_(std::move(image))
		{
		}

		void AddTransform(const glm::mat4& matrix) { transforms_.push_back(matrix); }
		void SetName(const std::string& name) { name_ = name; }
		const std::string& GetName() const { return name_; }
		void LoadTexture()
		{
			if (!image_)
			{
				ImageParser parser;
				image_ = parser.Parser(name_);
			}
		}

		const Image& GetTexture()
		{
			if (!image_)
			{
				LoadTexture();
			}

			return *image_;
		}
	protected:
		uint32_t tex_coord_index_;
		std::string name_;
		std::shared_ptr<Image> image_;
		std::vector<glm::mat4> transforms_;
	};
}

