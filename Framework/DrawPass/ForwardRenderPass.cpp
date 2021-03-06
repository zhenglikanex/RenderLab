#include "ForwardRenderPass.hpp"

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/GraphicsManager.hpp"

using namespace Aurora;

void ForwardRenderPass::Draw(Frame& frame)
{
	auto shader_program = g_app->GetEngine()->GetShaderManager()->GetDefaultShaderProgram(DefaultShaderIndex::Forward);
	auto& graphics_manager = g_app->GetEngine()->GetGraphicsManager();
	graphics_manager->UseShaderProgram(shader_program);
	graphics_manager->SetPerFrameConstants(frame.frame_context);
	graphics_manager->SetShadowMaps(frame);
	for (auto dbc : frame.batch_contexts)
	{
		graphics_manager->DrawBatch(*dbc);
	}
}