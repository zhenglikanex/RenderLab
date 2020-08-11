#include "GLFWApplication.hpp"

#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/InputManager.hpp"
#include "RHI/OpenGL/OpenGLGraphicsManager.hpp"

#include "GLFW/glfw3.h"

using namespace Aurora;

void GLFWApplication::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_LEFT:
			g_app->GetEngine()->GetInputManager()->LeftArrowKeyDown();
			break;
		case GLFW_KEY_RIGHT:
			g_app->GetEngine()->GetInputManager()->RightArrowKeyDown();
			break;
		case GLFW_KEY_UP:
			g_app->GetEngine()->GetInputManager()->UpArrowKeyDown();
			break;
		case GLFW_KEY_DOWN:
			g_app->GetEngine()->GetInputManager()->DownArrowKeyDown();
			break;
		}
	}

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_R:
			g_app->GetEngine()->GetInputManager()->ResetKeyDown();
			break;
			{
		default:
			break;
			}
		}
	}
}

GLFWApplication::GLFWApplication(GfxConfiguration& cfg,std::unique_ptr<Engine>&& engine)
    :BaseApplication(cfg,std::move(engine))
    ,window_(nullptr)
{
}

GLFWApplication::~GLFWApplication() 
{

}

bool GLFWApplication::Initialize()
{
    if (!BaseApplication::Initialize())
    {
        return false;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    window_ = glfwCreateWindow(config_.screen_width, config_.screen_height,config_.app_name.c_str(), nullptr, nullptr);
    if (window_ == nullptr)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    glfwSetKeyCallback(window_, GLFWApplication::KeyCallback);
    //glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    if (!engine_ || !engine_->Initialize())
    {
        std::cout << "Engine Initialize Failed !" << std::endl;
        return false;
    }

    return true;
}
void GLFWApplication::Finalize()
{
	if (engine_)
	{
		engine_->Finalize();
	}

    if (window_)
    {
        glfwDestroyWindow(window_);
    }
}

void GLFWApplication::Tick()
{
    BaseApplication::Tick();

    glfwSwapBuffers(window_);

    if (!glfwWindowShouldClose(window_))
    {
        glfwPollEvents();
    }
    else
    {
        Quit();
    }
}
