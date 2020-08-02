#pragma once

#include <memory>

#include "Framework/Common/IRuntimeModule.hpp"

namespace Aurora
{
	class GraphicsManager;
	class SceneManager;
	
	class Engine : public IRuntimeModule
	{
	public:
		Engine(std::unique_ptr<GraphicsManager>&& graphics_manager,std::unique_ptr<SceneManager>&& scene_manager);

		bool Initialize() override;
		void Finalize() override;

		void Tick() override;

		const std::unique_ptr<GraphicsManager>& GetGraphicsManager() const { return graphics_manager_; }
		const std::unique_ptr<SceneManager>& GetSceneManager() const { return scene_manager_; }
	private:
		std::unique_ptr<GraphicsManager> graphics_manager_;
		std::unique_ptr<SceneManager> scene_manager_;
	};
}