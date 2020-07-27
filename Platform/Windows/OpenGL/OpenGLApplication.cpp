#include "OpenGLApplication.hpp"

#include <stdio.h>
#include <tchar.h>

#include "Framework/Common/Engine.hpp"
#include "RHI/OpenGL/OpenGLGraphicsManager.hpp"
#include "3rdparty/glad/include/glad/glad_wgl.h"
#include "glad/glad_wgl.h"

using namespace Aurora;

GfxConfiguration config;
OpenGLApplication app(config);
IApplication* g_app = &app;

OpenGLApplication::OpenGLApplication(GfxConfiguration& cfg)
    :WindowsApplication(cfg)
{
    engine_ = std::make_unique<Engine>(
        std::make_unique<OpenGLGraphicsManager>(nullptr)
        );
}

bool OpenGLApplication::Initialize()
{
    if (!WindowsApplication::Initialize()) {
        printf("Windows Application initialize failed!");
    }
    else {
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = config_.red_bits + config_.green_bits + config_.blue_bits +config_.alpha_bits;
        pfd.cDepthBits = config_.depth_bits;
        pfd.iLayerType = PFD_MAIN_PLANE;

        HWND hWnd = this->GetMainWindow();
        HDC  hDC = GetDC(hWnd);
        // Set a temporary default pixel format.
        int nPixelFormat = ChoosePixelFormat(hDC, &pfd);
        if (nPixelFormat == 0) return false;

        if (SetPixelFormat(hDC, nPixelFormat, &pfd) != 1)
        {
            return false;
        }

        // Create a temporary rendering context.
        render_context_ = wglCreateContext(hDC);
        if (!render_context_)
        {
            return false;
        }

        // Set the temporary rendering context as the current rendering context for this window.
        if (wglMakeCurrent(hDC, render_context_) != 1)
        {
            return false;
        }

        if (!gladLoadWGL(hDC)) {
            printf("WGL initialize failed!\n");
        }
        else {
            printf("WGL initialize finished!\n");
        }

        if (!engine_ || !engine_->Initialize())
        {
            std::cout << "Engine Initialize Failed !" << std::endl;

            return false;
        }
    }

    return true;
}

void OpenGLApplication::Finalize()
{
    if (render_context_) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(render_context_);
        render_context_ = 0;
    }

    if (engine_)
    {
        engine_->Finalize();
    }

    WindowsApplication::Finalize();
}

void OpenGLApplication::Tick()
{
    WindowsApplication::Tick();
}

