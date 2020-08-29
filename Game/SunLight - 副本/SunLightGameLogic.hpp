#pragma once

#include "Framework/Common/GameLogic.hpp"

namespace Aurora
{
	class SunLightGameLogic : public GameLogic
	{
	public:
		bool Initialize() override;
		void Finalize() override;
		void Tick() override;

		void OnLeftKey() override;
	};
}
