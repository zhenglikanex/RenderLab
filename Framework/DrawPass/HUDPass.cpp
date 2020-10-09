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
	float left = 0.60f;

	for (uint32_t index = 0;index < frame.shadowmap_count;++index)
	{
		graphics_manager->DrawOverlay(frame.shadowmap,index,left, top, 0.35f, 0.35f);
		top -= 0.45f;
	}
#endif
}