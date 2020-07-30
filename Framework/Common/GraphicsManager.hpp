#pragma once

#include "Framework/Common/IRuntimeModule.hpp"

namespace Aurora
{
	class GraphicsManager : public IRuntimeModule
	{
	public:
		virtual ~GraphicsManager() = 0 {};

		virtual bool Initialize();
		virtual void Finalize();

		virtual void Tick();

		virtual void Clear();
		virtual void Draw();
	private:
	};
}