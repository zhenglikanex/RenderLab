#include <memory>
#include <iostream>


#include "IApplication.hpp"

using namespace Aurora;

extern IApplication* g_app;

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