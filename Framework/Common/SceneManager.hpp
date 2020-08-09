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
		
		bool IsSceneChanged();
		const Scene& GetSceneForRendering();
	protected:
		void LoadOgexScene(const char* ogex_scene_file_name);
	private:
		std::unique_ptr<Scene> scene_;
		bool dirty_flag_ = false;
	};
}