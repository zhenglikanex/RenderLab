#pragma once

#include "Framework/Common/BaseSceneObject.hpp"
#include "Framework/Common/SceneObjectTexture.hpp"

namespace Aurora
{
	class SceneObjectMaterial : public BaseSceneObject
	{
	public:
		SceneObjectMaterial(void)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeMaterial),
			name_(""),
			base_color_(glm::vec4(1.0f)),
			metallic_(0.0f),
			roughness_(0.0f),
			normal_(glm::vec3(0.0f, 0.0f, 1.0f)),
			specular_(glm::vec4(0.0f)),
			specular_power_(1.0f),
			ambient_occlusion_(1.0f),
			opacity_(glm::vec4(1.0f)),
			transparency_(glm::vec4(0.0f)),
			emission_(glm::vec4(0.0f)) {};
		SceneObjectMaterial(const std::string& name) : SceneObjectMaterial()
		{
			name_ = name;
		};

		const Color& GetBaseColor() const { return base_color_; }
		const Color& GetSpecularColor() const { return specular_; }
		const Parameter& GetSpecularPower() const { return specular_power_; }
		const Normal& GetNormal() const { return normal_; }
		void SetName(const std::string& name) { name_ = name; }
		void SetColor(const std::string& attrib, const glm::vec4& color)
		{
			if (attrib == "diffuse")
			{
				base_color_ = Color(color);
			}

			if (attrib == "specular")
			{
				specular_ = Color(color);
			}

			if (attrib == "emission")
			{
				emission_ = Color(color);
			}

			if (attrib == "opacity")
			{
				opacity_ = Color(color);
			}

			if (attrib == "transparency")
			{
				transparency_ = Color(color);
			}
		}

		void SetParam(const std::string& attrib, float param)
		{
			if (attrib == "specular_power")
			{
				specular_power_ = Parameter(param);
			}
		}

		void SetTexture(const std::string& attrib, const std::string& texture_name)
		{
			if (attrib == "diffuse")
			{
				base_color_ = std::make_shared<SceneObjectTexture>(texture_name);
			}

			if (attrib == "specular")
			{
				specular_ = std::make_shared<SceneObjectTexture>(texture_name);
			}

			if (attrib == "specular_power")
			{
				specular_power_ = std::make_shared<SceneObjectTexture>(texture_name);
			}

			if (attrib == "emission")
			{
				emission_ = std::make_shared<SceneObjectTexture>(texture_name);
			}

			if (attrib == "opactiy")
			{
				opacity_ = std::make_shared<SceneObjectTexture>(texture_name);
			}

			if (attrib == "transparency")
			{
				transparency_ = std::make_shared<SceneObjectTexture>(texture_name);
			}

			if (attrib == "normal")
			{
				normal_ = std::make_shared<SceneObjectTexture>(texture_name);
			}
		}

		void SetTexture(const std::string& attrib, const std::shared_ptr<SceneObjectTexture>& texture)
		{
			if (attrib == "diffuse")
			{
				base_color_ = texture;
			}

			if (attrib == "specular")
			{
				specular_ = texture;
			}

			if (attrib == "specular_power")
			{
				specular_power_ = texture;
			}

			if (attrib == "emission")
			{
				emission_ = texture;
			}

			if (attrib == "opactiy")
			{
				opacity_ = texture;
			}

			if (attrib == "transparency")
			{
				transparency_ = texture;
			}

			if (attrib == "normal")
			{
				normal_ = texture;
			}
		}

		void LoadTextures()
		{
			if (base_color_.ValueMap)
			{
				base_color_.ValueMap->LoadTexture();
			}

			if (normal_.ValueMap)
			{
				normal_.ValueMap->LoadTexture();
			}

			if (specular_.ValueMap)
			{
				specular_.ValueMap->LoadTexture();
			}

			if (specular_power_.ValueMap)
			{
				specular_power_.ValueMap->LoadTexture();
			}

			if (opacity_.ValueMap)
			{
				opacity_.ValueMap->LoadTexture();
			}

			if (transparency_.ValueMap)
			{
				transparency_.ValueMap->LoadTexture();
			}

			if (emission_.ValueMap)
			{
				emission_.ValueMap->LoadTexture();
			}
		}
	protected:
		std::string name_;
		Color base_color_;
		Parameter metallic_;
		Parameter roughness_;
		Normal normal_;
		Color specular_;
		Parameter specular_power_;
		Parameter ambient_occlusion_;
		Color opacity_;
		Color transparency_;
		Color emission_;
	};
}