#include "Framework/Common/GfxConfiguration.hpp"
#include "article_41_game_logic.hpp"

namespace Aurora
{
	GfxConfiguration config(8, 8, 8, 8, 24, 8, 0, 960, 540, "Billiard Game");
	GameLogic* g_pGameLogic = static_cast<GameLogic*>(new BilliardGameLogic);
}