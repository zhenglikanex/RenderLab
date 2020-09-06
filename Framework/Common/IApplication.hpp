#pragma once

#include "Framework/Common/IRuntimeModule.hpp"
#include "Framework/Common//GfxConfiguration.hpp"

namespace Aurora
{
	class Engine;

	class IApplication : public IRuntimeModule
	{
	public:
		virtual ~IApplication() = 0 {}

		virtual bool Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Tick() = 0;
		virtual float GetDt() = 0;
		virtual float GetElapse() = 0;
		virtual void Quit() = 0;
		virtual bool IsQuit() const = 0;

		virtual const GfxConfiguration& GetConfiguration() const = 0;
		virtual const std::unique_ptr<Engine>& GetEngine() const = 0;
	};

	extern IApplication* g_app;
}