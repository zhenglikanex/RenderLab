#include "GLFWApplication.hpp"

#include "Framework/Common/Engine.hpp"
#include "RHI/OpenGL/OpenGLGraphicsManager.hpp"

using namespace Aurora;

GfxConfiguration config;
GLFWApplication app(config);
IApplication* g_app = &app;

GLFWApplication::GLFWApplication(GfxConfiguration& cfg)
    :BaseApplication(cfg)
    ,window_(nullptr)
{
    engine_ = std::make_unique<Engine>(
        std::make_unique<OpenGLGraphicsManager>((GLADloadproc)glfwGetProcAddress)
        );
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
    if (window_)
    {
        glfwDestroyWindow(window_);
    }
}

void GLFWApplication::Tick()
{
    while (!glfwWindowShouldClose(window_))
    {
        glfwPollEvents();
    }

    BaseApplication::Tick();
}
