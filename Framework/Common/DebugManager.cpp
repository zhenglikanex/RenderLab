#include "DebugManager.hpp"

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/IPhysicsManager.hpp"

using namespace Aurora;

bool DebugManager::Initialize()
{
	return true;
}

void DebugManager::Finalize()
{

}

void DebugManager::Tick()
{
	if (draw_debug_info_)
	{
		DrawDebugInfo();
		//g_app->GetEngine()->GetPhysicsManager()->
	}
}

void DebugManager::ToggleDebugInfo()
{
	draw_debug_info_ = !draw_debug_info_;
}

void DebugManager::DrawDebugInfo()
{
#ifdef DEBUG
	glm::vec3 from(-1000.f, 0.0f, 0.0f);
	glm::vec3 to(1000.f, 0.0f, 0.0f);
	glm::vec3 color(1.0f, 0.0f, 0.0f);

	g_app->GetEngine()->GetGraphicsManager()->DrawLine(from, to, color);

	from = glm::vec3(0.0f, -1000.0f, 0.0f);
	to = glm::vec3(0.0f, 1000.0f, 0.0f);
	color = glm::vec3(0.0f, 1.0f, 0.0f);

	g_app->GetEngine()->GetGraphicsManager()->DrawLine(from, to, color);

	from = glm::vec3(0.0f, 0.0f, -1000.0f);
	to = glm::vec3(0.0f, 0.0f, 1000.0f);
	color = glm::vec3(0.0f, 0.0f, 1.0f);

	g_app->GetEngine()->GetGraphicsManager()->DrawLine(from, to, color);

#endif
}
