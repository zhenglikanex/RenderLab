#pragma once

#include <chrono>

#include "Framework/Common/IApplication.hpp"
#include "Framework/Common/GfxConfiguration.hpp"

namespace Aurora
{
	class Engine;

	class BaseApplication : public IApplication
	{
	public:
		BaseApplication(GfxConfiguration& cfg,std::unique_ptr<Engine>&& engine);
		virtual ~BaseApplication();

		bool Initialize() override;
		void Finalize() override;

		void Tick() override;

		void Quit() override { quit_ = true; }
		bool IsQuit() const override { return quit_; }

		const GfxConfiguration& GetConfiguration() const override { return config_; }
		const std::unique_ptr<Engine>& GetEngine() const override { return engine_; }
		float GetDt() override { return dt_; }
		float GetElapse() override { return elapse_; };
	protected:
		virtual void OnDraw() {}

		GfxConfiguration config_;
		std::unique_ptr<Engine> engine_;
	private:
		bool quit_;
		std::chrono::steady_clock::time_point last_time_;
		float dt_;
		float elapse_;
	};
}


