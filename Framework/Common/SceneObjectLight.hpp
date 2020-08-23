#pragma once

#include "Framework/Common/BaseSceneObject.hpp"

namespace Aurora
{
	//输入为光源的强度和到光源的距离，输出为光照强度的函数的指针
	using AttenFunc = float(*)(float /* Intensity */, float /* Distance */);

	inline float DefaultAttenFunc(float intensity, float distance)
	{
		return intensity / pow((1 + distance), 2.0f);
	}

	class SceneObjectLight : public BaseSceneObject
	{
	protected:
		SceneObjectLight(
			Color&& color = glm::vec4(1.0f),
			float intensity = 10.0f,
			AttenFunc atten_func = DefaultAttenFunc,
			bool cast_shadows = false)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeLight)
			, light_color_(std::move(color))
			, intensity_(intensity)
			, light_attenuation_(atten_func)
			, cast_shadows_(cast_shadows)
		{
		}
	public:
		void SetIfCastShadow(bool shadow) { cast_shadows_ = shadow; }

		void SetColor(const std::string& attrib, const glm::vec4& color)
		{
			if (attrib == "light")
			{
				light_color_ = Color(color);
			}
		}

		void SetParam(const std::string& attrib, float param)
		{
			if (attrib == "intensity")
			{
				intensity_ = param;
			}
		}

		void SetTexture(const std::string& attrib, const std::string& texture_name)
		{
			if (attrib == "projection")
			{
				texture_name_ = texture_name;
			}
		}

		void SetAttenuation(AttenFunc func)
		{
			light_attenuation_ = func;
		}

		const Color& GetColor() const { return light_color_; }
		float GetIntensity() const { return intensity_; }
	protected:
		Color light_color_;
		float intensity_;
		AttenFunc light_attenuation_;	//允許指定特殊的衰減的函數
		bool cast_shadows_;
		std::string texture_name_;
	};

	class SceneObjectInfiniteLight : public SceneObjectLight
	{
	public:
		using SceneObjectLight::SceneObjectLight;
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
		SceneObjectSpotLight() : SceneObjectLight(), cone_angle_(PI / 4.0f), penumbra_angle_(PI / 3.0f) { }
	protected:
		float cone_angle_;
		float penumbra_angle_;
	};
}