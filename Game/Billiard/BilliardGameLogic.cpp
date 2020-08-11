#include "BilliardGameLogic.hpp"

#include "Framework/Common/BaseApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/PhysicsManager.hpp"
#include "Framework/Utils/FileUtils.hpp"

using namespace Aurora;
using namespace std;

bool BilliardGameLogic::Initialize()
{
	//FileUtils::GetInstance()->AddSearchPath("Asset/");

	cout << "Biiliard Game Logic Initialize" << endl;
	cout << "Start Loading Game Scene" << endl;
	g_app->GetEngine()->GetSceneManager()->LoadScene("Scene/billiard.ogex");

	return true;
}

void BilliardGameLogic::Finalize()
{
	cout << "Biiliard Game Logic Finalize" << endl;
}

void BilliardGameLogic::Tick()
{

}

void BilliardGameLogic::OnLeftKey()
{
	/*auto ptr = g_app->GetEngine()->GetSceneManager()->GetSceneGeometryNode("pbb_cue");
	if (auto node = ptr.lock())
	{
		auto rigidBody = node->RigidBody();
		if (rigidBody)
		{
			g_pPhysicsManager->ApplyCentralForce(rigidBody, Vector3f(-100.0f, 0.0f, 0.0f));
		}
	}*/
}