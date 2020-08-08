#include "Engine.hpp"

#include <iostream>

#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/SceneManager.hpp"

using namespace Aurora;

Engine::Engine(std::unique_ptr<GraphicsManager>&& graphics_manager,std::unique_ptr<SceneManager>&& scene_manager)
	: graphics_manager_(std::move(graphics_manager))
	, scene_manager_(std::move(scene_manager))
{
	
}

bool Engine::Initialize()
{
	if (!scene_manager_ || !scene_manager_->Initialize())
	{
		std::cout << "SceneManager initialize failed!" << std::endl;

		return false;
	}

	scene_manager_->LoadScene("Scene/car.ogex");

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