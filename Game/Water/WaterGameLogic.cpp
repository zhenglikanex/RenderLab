#include "WaterGameLogic.hpp"

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/IPhysicsManager.hpp"
#include "Framework/Utils/FileUtils.hpp"

using namespace Aurora;
using namespace std;

bool WaterGameLogic::Initialize()
{
	//FileUtils::GetInstance()->AddSearchPath("Asset/");

	cout << "Biiliard Game Logic Initialize" << endl;
	cout << "Start Loading Game Scene" << endl;
	g_app->GetEngine()->GetSceneManager()->LoadScene("Scene/water.ogex");

	return true;
}

void WaterGameLogic::Finalize()
{
	cout << "Biiliard Game Logic Finalize" << endl;
}

void WaterGameLogic::Tick()
{

}

void WaterGameLogic::OnLeftKey()
{

	//g_app->GetEngine()->GetGraphicsManager()->SetPolygonMode(PolygonMode::kLine);
	auto ptr = g_app->GetEngine()->GetSceneManager()->GetSceneGeometryNode("Grid");
	if (auto node = ptr.lock())
	{
		node->MoveBy(glm::vec3(0.0f, 10.0f, 10.0f));
	}

}