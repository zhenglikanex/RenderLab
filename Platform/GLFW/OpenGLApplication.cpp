#include "Framework/Common/IApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/InputManager.hpp"
#include "Platform/GLFW/GLFWApplication.hpp"
#include "RHI/OpenGL/OpenGLGraphicsManager.hpp"
#include "Physics/My/MyPhysicsManager.hpp"

#include "GLFW/glfw3.h"

namespace Aurora
{
	extern GfxConfiguration config;
	GLFWApplication app(config,std::make_unique<Engine>(
			std::make_unique<OpenGLGraphicsManager>((GLADloadproc)glfwGetProcAddress),
			std::make_unique<SceneManager>(),
			std::make_unique<InputManager>(),
			std::make_unique<MyPhysicsManager>()
		));
	IApplication* g_app = &app;
}
