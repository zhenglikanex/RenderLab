#pragma once
#include <vector>
#include "Framework/Common/GeomMath.hpp"
#include "Framework/Common/Scene.hpp"
#include "Framework/Common/Guid.hpp"

namespace Aurora
{
	enum class LightType : int32_t
	{
		Point = 0,
		Spot = 1,
		Infinity = 2,
		Area = 3,
	};

	struct Light
	{
		xg::Guid light_guid;
		LightType light_type;
		glm::vec4 light_position;
		glm::vec4 light_color;
		glm::vec4 light_direction;
		glm::vec2 light_size;
		float light_intensity;
		AttenCurveType light_dist_atten_curve_type;
		float light_dist_atten_curve_params[5];
		AttenCurveType light_angle_atten_curve_type;
		float light_angle_atten_curve_params[5];
		bool light_cast_shadow;
		int32_t light_shadowmap_index;
		glm::mat4 light_vp;
		Light()
		{
			light_type = LightType::Point;
			light_position = { 0.0f,0.0f,0.0f,1.0f };
			light_size = { 0.0f,0.0f };
			light_color = { 1.0f,1.0f,1.0f,1.0f };
			light_direction = { 0.0f,0.0f,-1.0f,0.0f };
			light_intensity = 0.5f;
			light_dist_atten_curve_type = AttenCurveType::kNone;
			light_angle_atten_curve_type = AttenCurveType::kNone;
			light_cast_shadow = false;
			light_shadowmap_index = -1;
		}
	};

	struct DrawFrameContext {
		glm::mat4 world_matrix;
		glm::mat4 view_matrix;
		glm::mat4 projection_matrix;
		glm::vec3 ambient_color;
		std::vector<Light> lights;
	};
	
	struct DrawBatchContext {
		std::shared_ptr<SceneGeometryNode> node;
		std::shared_ptr<SceneObjectMaterial> material;
		glm::mat4 trans;

		virtual ~DrawBatchContext() = default;
	};

	struct Frame
	{
		DrawFrameContext frame_context;
		std::vector<std::shared_ptr<DrawBatchContext>> batch_contexts;
		intptr_t global_shadowmap;
		intptr_t shadowmap;
		intptr_t cube_shadowmap;
		uint32_t global_shadowmap_count;
		uint32_t shadowmap_count;
		uint32_t cube_shadowmap_count;

		Frame()
		{
			global_shadowmap = -1;
			shadowmap = -1;
			cube_shadowmap = -1;
			global_shadowmap_count = 0;
			shadowmap_count = 0;
			cube_shadowmap_count = 0;
		}
	};
}