#pragma once

#include "Framework/Common/IApplication.hpp"
#include "Framework/Common/GfxConfiguration.hpp"

namespace Aurora
{
	class Engine;

	class BaseApplication : public IApplication
	{
	public:
		BaseApplication(GfxConfiguration& cfg);
		virtual ~BaseApplication();

		bool Initialize() override;
		void Finalize() override;

		void Tick() override;

		void Quit() override { quit_ = true; }
		bool IsQuit() const override { return quit_; }
	protected:
		GfxConfiguration config_;
		std::unique_ptr<Engine> engine_;
	private:
		bool quit_;
	};
}


