#pragma once

#include "Framework/Common/BaseSceneObject.hpp"

namespace Aurora
{
	enum class AttenCurveType
	{
		kNone = 0,
		kLinear = 1,
		kSmooth = 2,
		kInverse = 3,
		kInverseSquare = 4
	};

	struct AttenCurve
	{
		AttenCurveType type;
		union AttenCurveParams
		{
			struct LinearParam { float begin_atten; float end_atten; } linear_params;
			struct SmoothParam { float begin_atten; float end_atten; } smooth_params;
			struct InverseParam { float scale; float offset; float k1; float kc; } inverse_params;
			struct InverseSquareParam { float scale; float offset; float kq; float k1; float kc; } inverse_squre_params;
		}u;

		AttenCurve() : type(AttenCurveType::kNone)
		{
		}
	};

	inline float DefaultAttenFunc(float intensity, float distance)
	{
		return intensity / pow((1 + distance), 2.0f);
	}

	class SceneObjectLight : public BaseSceneObject
	{
	protected:
		SceneObjectLight(SceneObjectType type) : BaseSceneObject(type),light_color_(glm::vec4(1.0f)),intensity_(1.0f),cast_shadows_(false)
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

		void SetDistanceAttenuation(AttenCurve curve)
		{
			light_distance_attenuation_ = curve;
		}

		const AttenCurve& GetDistanceAttenuation()
		{
			return light_distance_attenuation_;
		}

		const Color& GetColor() const { return light_color_; }
		float GetIntensity() const { return intensity_; }
	protected:
		Color light_color_;
		float intensity_;
		AttenCurve light_distance_attenuation_;	//允許指定特殊的衰減的函數
		bool cast_shadows_;
		std::string texture_name_;
	};

	class SceneObjectInfiniteLight : public SceneObjectLight
	{
	public:
		SceneObjectInfiniteLight() : SceneObjectLight(SceneObjectType::kSceneObjectTypeLightInfi) {}
		
	};

	class SceneObjectOmniLight : public SceneObjectLight
	{
	public:
		SceneObjectOmniLight() : SceneObjectLight(SceneObjectType::kSceneObjectTypeLightOmni) {}
	};

	class SceneObjectSpotLight : public SceneObjectLight
	{
	public:
		SceneObjectSpotLight() : SceneObjectLight(SceneObjectType::kSceneObjectTypeLightSpot) { }

		void SetAngleAttenuation(AttenCurve curve)
		{
			light_angle_attenuation_ = curve;
		}

		const AttenCurve& GetAngleAttenuation()
		{
			return light_angle_attenuation_;
		}
	protected:
		AttenCurve light_angle_attenuation_;
	};
}