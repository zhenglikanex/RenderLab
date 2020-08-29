#include "Engine.hpp"

#include <iostream>

#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/GameLogic.hpp"

using namespace Aurora;

Engine::Engine(std::unique_ptr<GraphicsManager>&& graphics_manager, std::unique_ptr<IShaderManager>&& shader_manager, std::unique_ptr<SceneManager>&& scene_manager, std::unique_ptr<InputManager>&& input_manager, std::unique_ptr<IPhysicsManager>&& physics_manager)
	: graphics_manager_(std::move(graphics_manager))
	, shader_manager_(std::move(shader_manager))
	, scene_manager_(std::move(scene_manager))
	, input_manager_(std::move(input_manager))
	, physics_manager_(std::move(physics_manager))
{
	debug_manager_ = std::make_unique<DebugManager>();
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

	if (!shader_manager_ || !shader_manager_->Initialize())
	{
		std::cout << "ShaderManager initalize failed!" << std::endl;

		return false;
	}

	if (!physics_manager_ || !physics_manager_->Initialize())
	{
		std::cout << "PhysicsManager initialize failed" << std::endl;

		return false;
	}

	if (!debug_manager_ || !debug_manager_->Initialize())
	{
		std::cout << "DebugManager initialize failed" << std::endl;

		return false;
	}

	if (!g_pGameLogic || !g_pGameLogic->Initialize()) 
	{
		std::cout << "GameLogic initialize faild" << std::endl;
		return false;
	}

	return true;
}

void Engine::Finalize()
{
	if (g_pGameLogic)
	{
		g_pGameLogic->Finalize();
	}

	if (physics_manager_)
	{
		physics_manager_->Finalize();
	}

	if (shader_manager_)
	{
		shader_manager_->Finalize();
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

	if (debug_manager_)
	{
		debug_manager_->Tick();
	}

	if (physics_manager_)
	{
		physics_manager_->Tick();
	}

	if (shader_manager_)
	{
		shader_manager_->Tick();
	}

	if (g_pGameLogic)
	{
		g_pGameLogic->Tick();
	}

	if (graphics_manager_)
	{
		graphics_manager_->Tick();
	}
}