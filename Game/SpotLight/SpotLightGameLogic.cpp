#include "SpotLightGameLogic.hpp"

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/IPhysicsManager.hpp"
#include "Framework/Utils/FileUtils.hpp"

using namespace Aurora;
using namespace std;

bool SpotLightGameLogic::Initialize()
{
	//FileUtils::GetInstance()->AddSearchPath("Asset/");

	cout << "Biiliard Game Logic Initialize" << endl;
	cout << "Start Loading Game Scene" << endl;
	g_app->GetEngine()->GetSceneManager()->LoadScene("Scene/splash.ogex");

	return true;
}

void SpotLightGameLogic::Finalize()
{
	cout << "Biiliard Game Logic Finalize" << endl;
}

void SpotLightGameLogic::Tick()
{
	
}

void SpotLightGameLogic::OnLeftKey()
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