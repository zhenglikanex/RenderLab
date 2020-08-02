#include <memory>
#include <iostream>


#include "IApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"

using namespace Aurora;

int main()
{
	if (!g_app->Initialize())
	{
		std::cout << "App Initialize failed!" << std::endl;
		return -1;
	}

	while (!g_app->IsQuit())
	{
		g_app->Tick();
	}

	g_app->Finalize();

	return 0;
}