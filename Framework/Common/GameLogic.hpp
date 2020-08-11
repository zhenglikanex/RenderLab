#pragma once

#include "Framework/Common/IRuntimeModule.hpp"

namespace Aurora
{
	class GameLogic : public IRuntimeModule
	{
	public:
		bool Initialize() override;
		void Finalize() override;
		void Tick() override;

		virtual void OnUpKeyDown();
		virtual void OnUpKeyUp();
		virtual void OnUpKey();

		virtual void OnDownKeyDown();
		virtual void OnDownKeyUp();
		virtual void OnDownKey();

		virtual void OnLeftKeyDown();
		virtual void OnLeftKeyUp();
		virtual void OnLeftKey();

		virtual void OnRightKeyDown();
		virtual void OnRightKeyUp();
		virtual void OnRightKey();
	private:
	};
	extern GameLogic* g_pGameLogic;
}