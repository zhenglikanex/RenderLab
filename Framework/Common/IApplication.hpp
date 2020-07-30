#pragma once

#include "Framework/Common/IRuntimeModule.hpp"

namespace Aurora
{
	class IApplication : public IRuntimeModule
	{
	public:
		virtual ~IApplication() = 0 {}

		virtual bool Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Tick() = 0;

		virtual void Quit() = 0;
		virtual bool IsQuit() const = 0;
	};

	extern IApplication* g_app;
}