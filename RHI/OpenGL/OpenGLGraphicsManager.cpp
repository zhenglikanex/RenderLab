#include "OpenGLGraphicsManager.hpp"

#include <iostream>
#include <fstream>

#include "Framework/Utils/FileHandle.hpp"


using namespace Aurora;

extern struct gladGLversionStruct GLVersion;

static void OutputShaderErrorMessage(uint32_t shader_id, const char* shader_filename)
{
    int log_size, i;
    char* info_log;
    std::ofstream fout;

    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_size);

    log_size++;

    info_log = new char[log_size];
    if (!info_log)
    {
        return;
    }

    glGetShaderInfoLog(shader_id, log_size, NULL, info_log);

    fout.open("shader_error.txt");
    for (i = 0; i < log_size; ++i)
    {
        fout << info_log[i];
    }

    fout.close();

    std::cerr << "Error compiling shader. Check shader_error.txt for message." << shader_filename << std::endl;

    return;
}

static void OutputLinkerErrorMessage(uint32_t program_id)
{
    int log_size, i;
    char* info_log;
    std::ofstream fout;
}

OpenGLGraphicsManager::OpenGLGraphicsManager(GLADloadproc loader)
    : loader_(loader)
{

}

bool OpenGLGraphicsManager::Initialize()
{
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

    printf("OpenGL Version %d.%d loaded\n", GLVersion.major, GLVersion.minor);

    if (GLAD_GL_VERSION_3_0) {
        // Set the depth buffer to be entirely cleared to 1.0 values.
        glClearDepth(1.0f);

        // Enable depth testing.
        glEnable(GL_DEPTH_TEST);

        // Set the polygon winding to front facing for the left handed system.
        glFrontFace(GL_CW);

        // Enable back face culling.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

	return true;
}

void OpenGLGraphicsManager::Finalize()
{

}

void OpenGLGraphicsManager::Tick()
{

}

void OpenGLGraphicsManager::Clear()
{

}

void OpenGLGraphicsManager::Draw()
{

}

bool OpenGLGraphicsManager::SetShaderParameters(float* world_matrix, float* view_matrix, float projection_matrix)
{
    return true;
}

bool OpenGLGraphicsManager::InitializeBuffers()
{
    return true;
}

void OpenGLGraphicsManager::RenderBuffers()
{

}

void OpenGLGraphicsManager::CalculateCameraPosition()
{

}

bool OpenGLGraphicsManager::InitializeShader(const char* vs_filename, const char* fs_filename)
{
    return true;
}