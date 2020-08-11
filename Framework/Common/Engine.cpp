#include "Engine.hpp"

#include <iostream>

#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/SceneManager.hpp"

using namespace Aurora;

Engine::Engine(std::unique_ptr<GraphicsManager>&& graphics_manager,std::unique_ptr<SceneManager>&& scene_manager,std::unique_ptr<InputManager>&& input_manager,std::unique_ptr<PhysicsManager>&& physics_manager)
	: graphics_manager_(std::move(graphics_manager))
	, scene_manager_(std::move(scene_manager))
	, input_manager_(std::move(input_manager))
	, physics_manager_(std::move(physics_manager))
{
	
}

bool Engine::Initialize()
{
	if (!input_manager_ || !input_manager_->Initialize())
	{
		std::cout << "InputManager initialize failed" << std::endl;
		
		return false;
	}

	if (!scene_manager_ || !scene_manager_->Initialize())
	{
		std::cout << "SceneManager initialize failed!" << std::endl;

		return false;
	}

	if (!graphics_manager_ || !graphics_manager_->Initialize())
	{
		std::cout << "GraphicsManager initialize failed!" << std::endl;

		return false;
	}

	if (!physics_manager_ || !physics_manager_->Initialize())
	{
		std::cout << "PhysicsManager initialize failed" << std::endl;

		return false;
	}

	scene_manager_->LoadScene("Scene/physics_1.ogex");

	return true;
}

void Engine::Finalize()
{
	if (physics_manager_)
	{
		physics_manager_->Finalize();
	}

	if (graphics_manager_)
	{
		graphics_manager_->Finalize();
	}

	if (scene_manager_)
	{
		scene_manager_->Finalize();
	}

	if (input_manager_)
	{
		input_manager_->Finalize();
	}
}

void Engine::Tick()
{
	if (input_manager_)
	{
		input_manager_->Tick();
	}

	if (scene_manager_)
	{
		scene_manager_->Tick();
	}

	if (physics_manager_)
	{
		physics_manager_->Tick();
	}

	if (graphics_manager_)
	{
		graphics_manager_->Tick();
	}
}