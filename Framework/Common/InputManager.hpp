#pragma once

#include "Framework/Common/IRuntimeModule.hpp"

namespace Aurora
{
	class InputManager : public IRuntimeModule
	{
	public:
        bool Initialize() override;
        void Finalize() override;
        void Tick() override;

        void UpArrowKeyDown();
        void UpArrowKeyUp();
        void DownArrowKeyDown();
        void DownArrowKeyUp();
        void LeftArrowKeyDown();
        void LeftArrowKeyUp();
        void RightArrowKeyDown();
        void RightArrowKeyUp();
		void ResetKeyDown();
#ifdef DEBUG
		void DebugKeyDown();
#endif
	private:
	};
}