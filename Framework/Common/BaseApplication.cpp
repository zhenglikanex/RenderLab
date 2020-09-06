#include "BaseApplication.hpp"

#include "Framework/Common/Engine.hpp"

using namespace Aurora;

BaseApplication::BaseApplication(GfxConfiguration& cfg,std::unique_ptr<Engine>&& engine)
	: config_(cfg)
	, engine_(std::move(engine))
{
	
}

BaseApplication::~BaseApplication()
{

}

bool BaseApplication::Initialize()
{
	quit_ = false;
	last_time_ = std::chrono::steady_clock::now();
	dt_ = 0;
	elapse_ = 0;
	std::cout << config_;

	return true;
}

void BaseApplication::Finalize()
{

}

void BaseApplication::Tick()
{
	auto now_time = std::chrono::steady_clock::now();
	dt_ = std::chrono::duration_cast<std::chrono::microseconds>(now_time - last_time_).count() / 1000000.0f;
	if (engine_)
	{
		engine_->Tick();
	}
	last_time_ = now_time;
	elapse_ += dt_;
}