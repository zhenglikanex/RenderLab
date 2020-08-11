#include "InputManager.hpp"

#include "Framework/Common/IApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/GeomMath.hpp"

using namespace Aurora;

bool InputManager::Initialize()
{
    return true;
}

void InputManager::Finalize()
{

}

void InputManager::Tick()
{

}

void InputManager::UpArrowKeyDown()
{
    g_app->GetEngine()->GetGraphicsManager()->WorldRotateX(PI / 60.0f);
}

void InputManager::UpArrowKeyUp()
{

}

void InputManager::DownArrowKeyDown()
{
    g_app->GetEngine()->GetGraphicsManager()->WorldRotateX(-PI / 60.0f);
}

void InputManager::DownArrowKeyUp()
{

}

void InputManager::LeftArrowKeyDown()
{
    g_app->GetEngine()->GetGraphicsManager()->WorldRotateY(PI / 60.0f);
}

void InputManager::LeftArrowKeyUp()
{

}

void InputManager::RightArrowKeyDown()
{
    g_app->GetEngine()->GetGraphicsManager()->WorldRotateY(-PI / 60.0f);
}

void InputManager::RightArrowKeyUp()
{

}

void InputManager::ResetKeyDown()
{
	g_app->GetEngine()->GetSceneManager()->ResetScene();
}