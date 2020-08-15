#pragma once

#include "Framework/Common/IRuntimeModule.hpp"

namespace Aurora
{
	class DebugManager : public IRuntimeModule
	{
	public:
		bool Initialize() override;
		void Finalize() override;
		void Tick() override;

		void ToggleDebugInfo();
		void DrawDebugInfo();
	protected:
		bool draw_debug_info_ = false;
	};
}