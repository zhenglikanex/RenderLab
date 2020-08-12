#pragma once

#include "Framework/Common/IRuntimeModule.hpp"
#include "Framework/Common/SceneParser.hpp"

namespace Aurora
{
	class SceneManager : public IRuntimeModule
	{
	public:
		virtual ~SceneManager();

		bool Initialize() override;
		void Finalize() override;

		void Tick() override;

		void LoadScene(const char* scene_file_name);
		void ResetScene();

		bool IsSceneChanged();
		const Scene& GetSceneForRendering();
		const Scene& GetSceneForPhysicsSimulation();
		void NotifySceneIsRenderingQueued();
		void NotifySceneIsPhysicsSimulationQueued();

		std::weak_ptr<SceneGeometryNode> GetSceneGeometryNode(const std::string& name);
	protected:
		void LoadOgexScene(const char* ogex_scene_file_name);
	private:
		std::shared_ptr<Scene> scene_;
		bool dirty_flag_ = false;
		bool rendering_queued_ = true;
		bool physics_simulation_queued_ = true;
	};
}