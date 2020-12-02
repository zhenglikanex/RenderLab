#include "HUDPass.hpp"

#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"


using namespace Aurora;

void HUDPass::Draw(Frame& frame)
{
	auto shader_program = g_app->GetEngine()->GetShaderManager()->GetDefaultShaderProgram(DefaultShaderIndex::Copy);
	auto& graphics_manager = g_app->GetEngine()->GetGraphicsManager();
	graphics_manager->UseShaderProgram(shader_program);
	
#ifdef DEBUG
	float top = 0.95f;
	float left = 0.70f;

	for (uint32_t index = 0;index < frame.shadowmap_count;++index)
	{
		graphics_manager->DrawTextureOverlay(frame.shadowmap,index,left, top, 0.25f, 0.25f);
		top -= 0.30f;
	}

	for (uint32_t i = 0; i < frame.global_shadowmap_count; i++)
	{
		graphics_manager->DrawTextureOverlay(frame.global_shadowmap, i, left, top, 0.25f, 0.25f);
		top -= 0.30f;
	}

	shader_program = g_app->GetEngine()->GetShaderManager()->GetDefaultShaderProgram(DefaultShaderIndex::CopyCube);
	graphics_manager->UseShaderProgram(shader_program);

	for (uint32_t i = 0; i < frame.cube_shadowmap_count; i++)
	{
		graphics_manager->DrawCubeMapOverlay(frame.cube_shadowmap, i, left, top, 0.25f, 0.25f);
		top -= 0.30f;
	}
#endif
}