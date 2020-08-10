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
		dirty_flag_ = true;
	}
}

void SceneManager::LoadOgexScene(const char* ogex_scene_file_name)
{
	std::string ogex_text = FileUtils::GetInstance()->OpenFileAndReadString(ogex_scene_file_name);

	OgexParser ogex_parser;
	scene_ = ogex_parser.Parser(ogex_text);
}

bool SceneManager::IsSceneChanged()
{
	return dirty_flag_;
}

const Scene& SceneManager::GetSceneForRendering()
{
	return *scene_;
}

void SceneManager::NotifySceneIsRenderingQueued()
{
	dirty_flag_ = false;
}