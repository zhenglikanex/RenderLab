#include "ShadowMapPass.hpp"

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/GraphicsManager.hpp"

using namespace Aurora;

void ShadowMapPass::Draw(Frame& frame)
{
	auto shader_program = g_app->GetEngine()->GetShaderManager()->GetDefaultShaderProgram(DefaultShaderIndex::ShadowMap);
	auto& graphics_manager = g_app->GetEngine()->GetGraphicsManager();
	graphics_manager->UseShaderProgram(shader_program);
	graphics_manager->SetPerFrameConstants(frame.frame_context);

	if (frame.global_shadowmap != -1)
	{
		graphics_manager->DestroyShadowMap(frame.global_shadowmap);
		frame.global_shadowmap_count = 0;
		frame.global_shadowmap = -1;
	}

	if (frame.shadowmap != -1)
	{
		graphics_manager->DestroyShadowMap(frame.shadowmap);
		frame.shadowmap_count = 0;
		frame.shadowmap = -1;
	}

	if (frame.cube_shadowmap != -1)
	{
		graphics_manager->DestroyShadowMap(frame.cube_shadowmap);
		frame.cube_shadowmap_count = 0;
		frame.cube_shadowmap = -1;
	}

	for (auto& light : frame.frame_context.lights)
	{
		if (light.light_cast_shadow)
		{
			switch (light.light_type)
			{
			case LightType::Point:
				frame.cube_shadowmap_count++;
				break;
			case LightType::Spot:
				frame.shadowmap_count++;
				break;
			case LightType::Area:
				frame.shadowmap_count++;
				break;
			case LightType::Infinity:
				frame.global_shadowmap_count++;
				break;
			default:
				assert(0);
			}
		}
	}
	const uint32_t kShadowMapWidth = 512;				// normal shadow map
	const uint32_t kShadowMapHeight = 512;				// normal shadow map
	const uint32_t kCubeShadowMapWidth = 512;			// cube shadow map
	const uint32_t kCubeShadowMapHeight = 512;			// cube shadow map
	const uint32_t kGlobalShadowMapWidth = 2048;		// shadow map for sun light
	const uint32_t kGlobalShadowMapHeight = 2048;		// shadow map for sun light
	
	
	frame.shadowmap = graphics_manager->GenerateShadowMapArray(kShadowMapWidth, kShadowMapHeight, frame.shadowmap_count);
	frame.global_shadowmap = graphics_manager->GenerateShadowMapArray(kGlobalShadowMapWidth, kGlobalShadowMapHeight, frame.global_shadowmap_count);
	frame.cube_shadowmap = graphics_manager->GenerateCubeShadowMapArray(kCubeShadowMapWidth, kCubeShadowMapHeight, frame.cube_shadowmap_count);

	uint32_t shadowmap_index = 0;
	uint32_t global_shadowmap_index = 0;
	uint32_t cube_shadowmap_index = 0;

	for (auto& light : frame.frame_context.lights)
	{
		if (light.light_cast_shadow)
		{
			intptr_t shadowmap;
			switch (light.light_type)
			{
			case LightType::Point:
				shadowmap = frame.cube_shadowmap;
				graphics_manager->BeginShadowMap(light, shadowmap, kCubeShadowMapWidth, kCubeShadowMapHeight,cube_shadowmap_index);
				light.light_shadowmap_index = cube_shadowmap_index++;
				break;
			case LightType::Spot:
				shadowmap = frame.shadowmap;
				graphics_manager->BeginShadowMap(light, shadowmap, kShadowMapWidth, kShadowMapHeight, shadowmap_index);
				light.light_shadowmap_index = shadowmap_index++;
				break;
			case LightType::Area:
				shadowmap = frame.shadowmap;
				graphics_manager->BeginShadowMap(light, shadowmap, kShadowMapWidth, kShadowMapHeight, shadowmap_index);
				light.light_shadowmap_index = shadowmap_index++;
				break;
			case LightType::Infinity:
				shadowmap = frame.global_shadowmap;
				graphics_manager->BeginShadowMap(light, shadowmap, kGlobalShadowMapWidth, kGlobalShadowMapHeight, global_shadowmap_index);
				light.light_shadowmap_index = global_shadowmap_index++;
				break;
			default:
				assert(0);
			}

			for (auto dbc : frame.batch_contexts)
			{
				graphics_manager->DrawBatchDepthOnly(*dbc);
			}

			graphics_manager->EndShadowMap(shadowmap, light.light_shadowmap_index);
		}
	}
}