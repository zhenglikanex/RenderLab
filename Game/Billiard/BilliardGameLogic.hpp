#pragma once

#include "Framework/Common/GameLogic.hpp"

namespace Aurora
{
	class BilliardGameLogic : public GameLogic
	{
	public:
		bool Initialize() override;
		void Finalize() override;
		void Tick() override;

		virtual void OnLeftKey();
	};
}
