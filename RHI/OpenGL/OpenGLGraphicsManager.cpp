#include "OpenGLGraphicsManager.hpp"

using namespace Aurora;

extern struct gladGLversionStruct GLVersion;

OpenGLGraphicsManager::OpenGLGraphicsManager(GLADloadproc loader)
    : loader_(loader)
{

}
bool OpenGLGraphicsManager::Initialize()
{
	if (!GraphicsManager::Initialize())
	{
		return false;
	}

	if (loader_)
	{
		if (gladLoadGLLoader(loader_) != 1)
		{
			std::cout << "glad load failed !" << std::endl;
			return false;
		}
	}
	else {
		if (gladLoadGL() != 1)
		{
			std::cout << "glad load failed !" << std::endl;
			return false;
		}
	}

	std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << " loaded" << std::endl;

	if (GLAD_GL_VERSION_3_0) {
		// Set the depth buffer to be entirely cleared to 1.0 values.
		glClearDepth(1.0f);

		// Enable depth testing.
		glEnable(GL_DEPTH_TEST);

		// Set the polygon winding to front facing for the left handed system.
		glFrontFace(GL_CCW);

		// Enable back face culling.
		//glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	
	return true;
}