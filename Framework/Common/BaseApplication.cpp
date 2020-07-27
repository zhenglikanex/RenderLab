#include "BaseApplication.hpp"

#include "Framework/Common/Engine.hpp"

using namespace Aurora;

BaseApplication::BaseApplication(GfxConfiguration& cfg)
	:config_(cfg)
{
	
}

BaseApplication::~BaseApplication()
{

}

bool BaseApplication::Initialize()
{
	quit_ = false;

	std::cout << config_;

	return true;
}

void BaseApplication::Finalize()
{

}

void BaseApplication::Tick()
{
	if (engine_)
	{
		engine_->Tick();
	}
}