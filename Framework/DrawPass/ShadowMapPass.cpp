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

	if (frame.shadowmap != -1)
	{
		graphics_manager->DestroyShadowMap(frame.shadowmap);
	}

	frame.shadowmap_count = 0;

	uint32_t cast_shadow_count = 0;
	for (auto& light : frame.frame_context.lights)
	{
		if (light.light_cast_shadow)
		{
			++cast_shadow_count;
		}
	}

	uint32_t index = 0;
	if (cast_shadow_count > 0)
	{
		intptr_t shadow_map = graphics_manager->GenerateShadowMapArray(frame.frame_context.lights.size());

		frame.shadowmap = shadow_map;
		frame.shadowmap_count = cast_shadow_count;

		for (auto& light : frame.frame_context.lights)
		{
			if (light.light_cast_shadow)
			{
				//generate shadow map here
				graphics_manager->BeginShadowMap(light, shadow_map,index);


				for (auto dbc : frame.batch_contexts)
				{
					graphics_manager->DrawBatchDepthOnly(*dbc);
				}

				graphics_manager->EndShadowMap(shadow_map,index);

				++index;
			}
		}
	}
}