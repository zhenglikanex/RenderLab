#include "AreaLightGameLogic.hpp"

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/IPhysicsManager.hpp"
#include "Framework/Utils/FileUtils.hpp"

using namespace Aurora;
using namespace std;

bool AreaLightGameLogic::Initialize()
{
	//FileUtils::GetInstance()->AddSearchPath("Asset/");

	cout << "Biiliard Game Logic Initialize" << endl;
	cout << "Start Loading Game Scene" << endl;
	g_app->GetEngine()->GetSceneManager()->LoadScene("Scene/area_light.ogex");

	return true;
}

void AreaLightGameLogic::Finalize()
{
	cout << "Biiliard Game Logic Finalize" << endl;
}

void AreaLightGameLogic::Tick()
{

}

void AreaLightGameLogic::OnLeftKey()
{
	auto ptr = g_app->GetEngine()->GetSceneManager()->GetSceneLightNode("Lamp.001");
	if (auto node = ptr.lock())
	{
		node->MoveBy(glm::vec3(0.0f, -0.1f, 0.0f));
	}
}

void Aurora::AreaLightGameLogic::OnRightKey()
{
	auto ptr = g_app->GetEngine()->GetSceneManager()->GetSceneLightNode("Lamp.001");
	if (auto node = ptr.lock())
	{
		node->MoveBy(glm::vec3(0.0f, 0.1f, 0.0f));
	}
}


void AreaLightGameLogic::OnUpKey()
{
	auto ptr = g_app->GetEngine()->GetSceneManager()->GetSceneLightNode("Lamp.001");
	if (auto node = ptr.lock())
	{
		node->MoveBy(glm::vec3(0.0f, 0.0f, 0.1f));
	}
}

void Aurora::AreaLightGameLogic::OnDownKey()
{
	auto ptr = g_app->GetEngine()->GetSceneManager()->GetSceneLightNode("Lamp.001");
	if (auto node = ptr.lock())
	{
		node->MoveBy(glm::vec3(0.0f, 0.0f, -0.1f));
	}
}

