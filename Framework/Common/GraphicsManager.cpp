#include "GraphicsManager.hpp"

#include <iostream>

#include "Framework/Common/IApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/DrawPass/ForwardRenderPass.hpp"
#include "Framework/DrawPass/ShadowMapPass.hpp"
#include "Framework/DrawPass/HUDPass.hpp"

using namespace Aurora;

#define MAX_LIGHTS 55

bool GraphicsManager::Initialize()
{
	frames_.resize(kFrameCount);

	InitConstants();

	draw_passes_.push_back(std::make_shared<ShadowMapPass>());
	draw_passes_.push_back(std::make_shared<ForwardRenderPass>());
	draw_passes_.push_back(std::make_shared<HUDPass>());

	return true;
}

void GraphicsManager::Finalize()
{

}

void GraphicsManager::Tick()
{
	if (g_app->GetEngine()->GetSceneManager()->IsSceneChanged())
	{
		ClearBuffers();
		auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
		InitializeBuffers(scene);

		g_app->GetEngine()->GetSceneManager()->NotifySceneIsRenderingQueued();
	}

	UpdateConstants();

	Clear();
	Draw();
	ClearDebugBuffers();
}

void GraphicsManager::Clear()
{

}

void GraphicsManager::Draw()
{
	auto& frame = frames_[frame_index_];
	
	for (auto draw_pass : draw_passes_)
	{
		draw_pass->Draw(frame);
	}
}

#ifdef DEBUG
void Aurora::GraphicsManager::DrawLine(const glm::vec3 & from, const glm::vec3 & to, const glm::vec3 & color)
{
}

void Aurora::GraphicsManager::DrawBox(const glm::vec3 & bbMin, const glm::vec3 bbMax, const glm::vec3 & color)
{
}

void GraphicsManager::DrawOverlay(const intptr_t shadowmap,uint32_t layer,float vp_left, float vp_top, float vp_width, float vp_height)
{

}

void Aurora::GraphicsManager::ClearDebugBuffers()
{
}
#endif
void GraphicsManager::InitConstants()
{
	frames_[frame_index_].frame_context.world_matrix = glm::identity<glm::mat4>();
}

void GraphicsManager::InitializeBuffers(const Scene& scene)
{
	std::cout << "[RHI] void GraphicsManager::InitializeBuffers()" << std::endl;
}

void GraphicsManager::ClearBuffers()
{
	std::cout << "[RIH] void GraphicsManager::ClearBuffers()" << std::endl;
}

void GraphicsManager::CalculateCameraMatrix()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
	auto camera_node = scene.GetFirstCameraNode();
	DrawFrameContext& frame_context = frames_[frame_index_].frame_context;
	if (camera_node)
	{
		frame_context.view_matrix = glm::inverse(*camera_node->GetCalculatedTransform());
	}
	else
	{
		glm::vec3 position = { 0,-5,0 }, look_at = { 0,0,0 }, up = { 0,0,1 };
		frame_context.view_matrix = glm::lookAtLH(position, look_at, up);
	}

	float fov = PI / 2.0f;
	float near_clip_distance = 1.0f;
	float far_clip_distance = 100.0f;

	if (camera_node)
	{
		auto camera = scene.GetCamera(camera_node->GetSceneObjectRef());
		fov = std::dynamic_pointer_cast<SceneObjectPerspectiveCamera>(camera)->GetFov();
		near_clip_distance = 1.0f;
		far_clip_distance = 100.0f;
	}

	const GfxConfiguration& conf = g_app->GetConfiguration();
	frame_context.projection_matrix = glm::perspectiveFovRH(fov, (float)conf.screen_width, (float)conf.screen_height, near_clip_distance, far_clip_distance);
}

void GraphicsManager::CalculateLights()
{
	DrawFrameContext& frame_context = frames_[frame_index_].frame_context;
	frame_context.ambient_color = { 0.05f,0.05f,0.05f };
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
	frame_context.lights.clear();
	for (auto& it : scene.LightNodes)
	{
		if (frame_context.lights.size() >= MAX_LIGHTS)
		{
			break;
		}

		auto light_node = it.second;

		auto light = scene.GetLight(light_node->GetSceneObjectRef());
		if (light)
		{
			Light light_param;
			light_param.light_guid = light->guid();
			auto trans_ptr = light_node->GetCalculatedTransform();
			light_param.light_position = (*trans_ptr) * light_param.light_position;
			light_param.light_direction = (*trans_ptr) * light_param.light_direction;
			light_param.light_color = light->GetColor().Value;
			light_param.light_intensity = light->GetIntensity();
			const AttenCurve& atten_curve = light->GetDistanceAttenuation();
			light_param.light_dist_atten_curve_type = atten_curve.type;
			memcpy(light_param.light_dist_atten_curve_params, &atten_curve.u, sizeof(atten_curve.u));
			light_param.light_cast_shadow = light->GetIfCastShadow();

			glm::vec3 position = light_param.light_position;
			glm::vec3 look_at = position + glm::vec3(light_param.light_direction);
			glm::vec3 up = { 0.0f,0.0f,1.0f };

			glm::mat4 view = glm::lookAtRH(position, look_at, up);
			float near_clip_dist = 1.0f;
			float far_clip_dist = 100.0f;
			glm::mat4 projection = glm::identity<glm::mat4>();

			if (light->GetType() == SceneObjectType::kSceneObjectTypeLightInfi)
			{
				light_param.light_type = LightType::Infinity;

				glm::vec4 target = { 0.0f,0.0f,0.0f,1.0f };
				auto camera_node = scene.GetFirstCameraNode();
				if (camera_node)
				{
					auto camera = scene.GetCamera(camera_node->GetSceneObjectRef());
					near_clip_dist = camera->GetNearClipDistance();
					far_clip_dist = camera->GetFarClipDistance();

					target[2] = -(0.75f * near_clip_dist + 0.25f * far_clip_dist);

					auto tran = *camera_node->GetCalculatedTransform();
					target = tran * target;
				}

				light_param.light_position = target - light_param.light_direction * far_clip_dist;
				glm::vec3 position = light_param.light_position;
				glm::vec3 look_at = target;
				glm::vec3 up = { 0.0f,0.0f,1.0f };
				if (std::abs(light_param.light_direction.x) <= 0.2f && std::abs(light_param.light_direction.y) <= 0.2f)
				{
					up = { 0.1f,0.1f,1.0f };
				}
				view = glm::lookAtRH(position, look_at, up);
				float sm_half_dist = std::min(far_clip_dist * 0.25f, 800.0f);
				projection = glm::orthoRH(-sm_half_dist, sm_half_dist, -sm_half_dist, sm_half_dist, near_clip_dist, far_clip_dist + sm_half_dist);

				// ¸æËßshaderÊÇinfinity light
				light_param.light_position[3] = 0.0f;
			}
			else {
				glm::vec3 position = light_param.light_position;
				glm::vec3 look_at = light_param.light_position + light_param.light_direction;
				glm::vec3 up = { 0.0f,0.0f,1.0f };
				if (std::abs(light_param.light_direction.x) <= 0.1f
					&& std::abs(light_param.light_direction.y) <= 0.1f)
				{
					up = { 0.0f,0.707f,0.707f };
				}
				view = glm::lookAt(position, look_at, up);


				if (light->GetType() == SceneObjectType::kSceneObjectTypeLightSpot)
				{
					light_param.light_type = LightType::Spot;

					auto spot_light = std::dynamic_pointer_cast<SceneObjectSpotLight>(light);
					const AttenCurve& angle_atten_curve = spot_light->GetAngleAttenuation();
					light_param.light_angle_atten_curve_type = angle_atten_curve.type;
					memcpy(light_param.light_angle_atten_curve_params, &angle_atten_curve.u, sizeof(angle_atten_curve.u));

					float fov = light_param.light_angle_atten_curve_params[1] * 2.0f;
					float screen_aspect = 1.0f;
					projection = glm::perspectiveFovRH(fov, 100.0f, 100.0f, near_clip_dist, far_clip_dist);
				}
				else if (light->GetType() == SceneObjectType::kSceneObjectTypeLightArea)
				{
					light_param.light_type = LightType::Area;

					auto area_light = std::dynamic_pointer_cast<SceneObjectAreaLight>(light);
					light_param.light_size = area_light->GetDimension();
				}
			}

			light_param.light_vp = projection * view;

			frame_context.lights.emplace_back(std::move(light_param));
		}
	}
}
void GraphicsManager::UpdateConstants()
{
	auto& frame = frames_[frame_index_];
	for (auto dbc : frame.batch_contexts)
	{
		if (void* rigidbody = dbc->node->RigidBody())
		{
			auto simulated_result = g_app->GetEngine()->GetPhysicsManager()->GetRigidBodyTransform(rigidbody);

			// replace the translation part of the matrix with simlation result directly
			//trans[3] = glm::vec4(0.0f,0.0f,0.0f,trans[3].w);

			// apply the rotation part of the simlation result
			dbc->trans = glm::identity<glm::mat4>();
			dbc->trans[0] = simulated_result[0];
			dbc->trans[1] = simulated_result[1];
			dbc->trans[2] = simulated_result[2];

			dbc->trans[3] = glm::vec4(simulated_result[3].x, simulated_result[3].y, simulated_result[3].z, dbc->trans[3].w);
		}
		else
		{
			dbc->trans = *dbc->node->GetCalculatedTransform();
		}
	}

	CalculateCameraMatrix();
	CalculateLights();
}

void GraphicsManager::UseShaderProgram(intptr_t shader_program)
{
	
}

void GraphicsManager::SetPerFrameConstants(const DrawFrameContext& context)
{
	
}

void GraphicsManager::DrawBatch(const DrawBatchContext& context)
{

}

void GraphicsManager::DrawBatchDepthOnly(const DrawBatchContext& context)
{

}

intptr_t GraphicsManager::GenerateShadowMap(const uint32_t width, const uint32_t height)
{
	return 0;
}

intptr_t GraphicsManager::GenerateCubeShadowMapArray(const uint32_t width, const uint32_t height, const uint32_t count)
{
	return 0;
}

intptr_t GraphicsManager::GenerateShadowMapArray(const uint32_t width, const uint32_t height, const uint32_t count)
{
	return 0;
}
void GraphicsManager::BeginShadowMap(const Light& light, const intptr_t shadowmap, const uint32_t width, const uint32_t height, const uint32_t layer_index)
{

}

void GraphicsManager::EndShadowMap(const intptr_t shadowmap, const uint32_t layer_index)
{

}

void GraphicsManager::SetShadowMap(const intptr_t shadowmap)
{

}

void GraphicsManager::SetGlobalShadowMap(const intptr_t shadowmap)
{

}

void GraphicsManager::SetShadowMaps(const Frame& frame)
{
	
}

void GraphicsManager::DestroyShadowMap(intptr_t& shadowmap)
{

}