#include "Framework/Common/GfxConfiguration.hpp"
#include "WaterGameLogic.hpp"

namespace Aurora
{
	GfxConfiguration config(8, 8, 8, 8, 24, 8, 0, 1600, 1200, "SunLight Game");
	GameLogic* g_pGameLogic = static_cast<GameLogic*>(new WaterGameLogic);
}