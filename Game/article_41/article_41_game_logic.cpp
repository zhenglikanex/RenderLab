#include "article_41_game_logic.hpp"

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/IPhysicsManager.hpp"
#include "Framework/Utils/FileUtils.hpp"

using namespace Aurora;
using namespace std;

bool Article41GameLogic::Initialize()
{
	//FileUtils::GetInstance()->AddSearchPath("Asset/");

	cout << "Biiliard Game Logic Initialize" << endl;
	cout << "Start Loading Game Scene" << endl;
	g_app->GetEngine()->GetSceneManager()->LoadScene("Scene/test.ogex");

	return true;
}

void Article41GameLogic::Finalize()
{
	cout << "Biiliard Game Logic Finalize" << endl;
}

void Article41GameLogic::Tick()
{

}

void Article41GameLogic::OnLeftKey()
{
	auto ptr = g_app->GetEngine()->GetSceneManager()->GetSceneGeometryNode("pbb_cue");
	if (auto node = ptr.lock())
	{
		auto rigidBody = node->RigidBody();
		if (rigidBody)
		{
			g_app->GetEngine()->GetPhysicsManager()->ApplyCentralForce(rigidBody, glm::vec3(-100.0f, 0.0f, 0.0f));
		}
	}
}