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
	}
}

void SceneManager::LoadOgexScene(const char* ogex_scene_file_name)
{
	std::string ogex_text = FileUtils::GetInstance()->OpenFileAndReadString(ogex_scene_file_name);

	OgexParser ogex_parser;
	scene_ = ogex_parser.Parser(ogex_text);
}

const Scene& SceneManager::GetSceneForRendering()
{
	return *scene_;
}