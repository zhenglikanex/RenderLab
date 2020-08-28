#pragma once

#include "Framework/Common/GameLogic.hpp"

namespace Aurora
{
	class AreaLightGameLogic : public GameLogic
	{
	public:
		bool Initialize() override;
		void Finalize() override;
		void Tick() override;

		void OnUpKey() override;
		void OnDownKey() override;
		void OnLeftKey() override;
		void OnRightKey() override;
	};
}
