#include "SceneManager.hpp"
#include "Framework/Common/OGEX.hpp"
#include "Framework/Utils/FileUtils.hpp"

using namespace Aurora;

SceneManager::~SceneManager()
{

}

bool SceneManager::Initialize()
{
	scene_ = std::make_unique<Scene>("scene");
	return true;
}

void SceneManager::Finalize()
{

}

void SceneManager::Tick()
{

}

void SceneManager::LoadScene(const char* scene_file_name)
{
	LoadOgexScene(scene_file_name);
	if (scene_)
	{
		scene_->LoadResource();
		ResetScene();
	}
}

void SceneManager::LoadOgexScene(const char* ogex_scene_file_name)
{
	std::string ogex_text = FileUtils::GetInstance()->OpenFileAndReadString(ogex_scene_file_name);

	OgexParser ogex_parser;
	scene_ = ogex_parser.Parser(ogex_text);
}

void SceneManager::ResetScene()
{
	dirty_flag_ = true;
	rendering_queued_ = false;
	physics_simulation_queued_ = false;
}

bool SceneManager::IsSceneChanged()
{
	return dirty_flag_;
}

const Scene& SceneManager::GetSceneForRendering()
{
	return *scene_;
}

const Scene& SceneManager::GetSceneForPhysicsSimulation()
{
	return *scene_;
}

void SceneManager::NotifySceneIsRenderingQueued()
{
	rendering_queued_ = true;

	if (physics_simulation_queued_)
	{
		dirty_flag_ = false;
	}
}

void SceneManager::NotifySceneIsPhysicsSimulationQueued()
{
	physics_simulation_queued_ = true;

	if (rendering_queued_)
	{
		dirty_flag_ = false;
	}
}

std::weak_ptr<SceneGeometryNode> SceneManager::GetSceneGeometryNode(const std::string& name)
{
	auto iter = scene_->LUT_name_GeometryNodes.find(name);
	if (iter != scene_->LUT_name_GeometryNodes.end())
	{
		return iter->second;
	}
	else 
	{
		return std::weak_ptr<SceneGeometryNode>();
	}
}