#include "Engine.hpp"

#include <iostream>

#include "Framework/Common/GraphicsManager.hpp"

using namespace Aurora;

Engine::Engine(std::unique_ptr<GraphicsManager>&& graphics_manager)
	:graphics_manager_(std::move(graphics_manager))
{
	
}

bool Engine::Initialize()
{
	if (!graphics_manager_ || !graphics_manager_->Initialize())
	{
		std::cout << "GraphicsManager initialize failed!" << std::endl;

		return false;
	}

	return true;
}

void Engine::Finalize()
{
	if (graphics_manager_)
	{
		graphics_manager_->Finalize();
	}
}

void Engine::Tick()
{
	if (graphics_manager_)
	{
		graphics_manager_->Tick();
	}
}