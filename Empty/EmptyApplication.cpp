#include <iostream>
#include <memory>

#include "Framework/Common/BaseApplication.hpp"

using namespace Aurora;

int main()
{
	GfxConfiguration config;
	std::unique_ptr<BaseApplication> app = std::make_unique<BaseApplication>(config);
	if (!app->Initialize())
	{
		std::cerr << "App Initialize" << std::endl;
		return -1;
	}

	while (!app->IsQuit())
	{
		app->Tick();
	}

	app->Finalize();

	return 0;
}