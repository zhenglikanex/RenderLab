#include "Framework/Common/GfxConfiguration.hpp"
#include "AreaLightGameLogic.hpp"

namespace Aurora
{
	GfxConfiguration config(8, 8, 8, 8, 24, 8, 0, 960, 540, "SunLight Game");
	GameLogic* g_pGameLogic = static_cast<GameLogic*>(new AreaLightGameLogic);
}