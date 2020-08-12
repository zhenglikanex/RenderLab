#pragma once

#include <memory>

#include "Framework/Common/IRuntimeModule.hpp"
#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/InputManager.hpp"
#include "Framework/Common/IPhysicsManager.hpp"

namespace Aurora
{
	class Engine : public IRuntimeModule
	{
	public:
		Engine(std::unique_ptr<GraphicsManager>&& graphics_manager,std::unique_ptr<SceneManager>&& scene_manager,std::unique_ptr<InputManager>&& input_manager,std::unique_ptr<IPhysicsManager>&& physics_manager);

		bool Initialize() override;
		void Finalize() override;

		void Tick() override;

		const std::unique_ptr<GraphicsManager>& GetGraphicsManager() const { return graphics_manager_; }
		const std::unique_ptr<SceneManager>& GetSceneManager() const { return scene_manager_; }
		const std::unique_ptr<InputManager>& GetInputManager() const { return input_manager_; }
		const std::unique_ptr<IPhysicsManager>& GetPhysicsManager() const { return physics_manager_; }
	private:
		std::unique_ptr<GraphicsManager> graphics_manager_;
		std::unique_ptr<SceneManager> scene_manager_;
		std::unique_ptr<InputManager> input_manager_;
		std::unique_ptr<IPhysicsManager> physics_manager_;
	};
}