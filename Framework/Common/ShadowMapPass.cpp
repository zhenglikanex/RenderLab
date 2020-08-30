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

	for (auto light : frame.frame_context.lights)
	{
		if (light.cast_shadow)
		{
			//generate shadow map here
			intptr_t shadow_map;
			graphics_manager->GenerateShadowMap(light);
			if (frame.shadow_maps.find(light.light_guid) == frame.shadow_maps.end())
			{
				shadow_map = graphics_manager->GenerateShadowMap(light);
				frame.shadow_maps[light.light_guid] = shadow_map;
			}
			else
			{
				shadow_map = frame.shadow_maps[light.light_guid];
			}
			
			graphics_manager->BeginShadowMap(light, shadow_map);


			for (auto dbc : frame.batch_contexts)
			{
				graphics_manager->DrawBatchDepthOnly(*dbc);
			}

			graphics_manager->EndShadowMap(shadow_map);
		}
	}
}