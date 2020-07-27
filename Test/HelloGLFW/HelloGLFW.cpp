// include the basic windows header file
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <GL/gl.h>
#include <fstream>

#include "math.h"

#include<GLFW/glfw3.h>

using namespace std;

/////////////
// DEFINES //
/////////////
#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB  0x2092
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_TEXTURE0                       0x84C0
#define GL_BGRA                           0x80E1
#define GL_ELEMENT_ARRAY_BUFFER           0x8893

//////////////
// TYPEDEFS //
//////////////
typedef BOOL (WINAPI   * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef HGLRC (WINAPI  * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL (WINAPI   * PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef void (APIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRY * PFNGLBUFFERDATAPROC) (GLenum target, ptrdiff_t size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY * PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint(APIENTRY * PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint(APIENTRY * PFNGLCREATESHADERPROC) (GLenum type);
typedef void (APIENTRY * PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRY * PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (APIENTRY * PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef void (APIENTRY * PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef GLint(APIENTRY * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const char *name);
typedef void (APIENTRY * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog);
typedef void (APIENTRY * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog);
typedef void (APIENTRY * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY * PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRY * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const char* *string, const GLint *length);
typedef void (APIENTRY * PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (APIENTRY * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const char *name);
typedef GLint(APIENTRY * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const char *name);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (APIENTRY * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (APIENTRY * PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void (APIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY * PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY * PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat *value);

PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

typedef struct VertexType
{
    VectorType position;
    VectorType color;
} VertexType;

HDC     g_deviceContext = 0;
HGLRC   g_renderingContext = 0;
char    g_videoCardDescription[128];

const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

int     g_vertexCount, g_indexCount;
unsigned int g_vertexArrayId, g_vertexBufferId, g_indexBufferId;

unsigned int g_vertexShader;
unsigned int g_fragmentShader;
unsigned int g_shaderProgram;

const char VS_SHADER_SOURCE_FILE[] = "color.vs";
const char PS_SHADER_SOURCE_FILE[] = "color.ps";

float g_positionX = 0, g_positionY = 0, g_positionZ = -10;
float g_rotationX = 0, g_rotationY = 0, g_rotationZ = 0;
float g_worldMatrix[16];
float g_viewMatrix[16];
float g_projectionMatrix[16];

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool InitializeOpenGL(int screenWidth, int screenHeight, float screenDepth, float screenNear, bool vsync)
{
        unsigned int formatCount;
        int result;

        float fieldOfView, screenAspect;
        char *vendorString, *rendererString;

        // Set the depth buffer to be entirely cleared to 1.0 values.
        glClearDepth(1.0f);

        // Enable depth testing.
        glEnable(GL_DEPTH_TEST);

        // Set the polygon winding to front facing for the left handed system.
        glFrontFace(GL_CW);

        // Enable back face culling.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Initialize the world/model matrix to the identity matrix.
        BuildIdentityMatrix(g_worldMatrix);

        // Set the field of view and screen aspect ratio.
        fieldOfView = PI / 4.0f;
        screenAspect = (float)screenWidth / (float)screenHeight;

        // Build the perspective projection matrix.
        BuildPerspectiveFovLHMatrix(g_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

        // Get the name of the video card.
        vendorString = (char*)glGetString(GL_VENDOR);
        rendererString = (char*)glGetString(GL_RENDERER);
        // Store the video card name in a class member variable so it can be retrieved later.
        strcpy_s(g_videoCardDescription, vendorString);
        strcat_s(g_videoCardDescription, " - ");
        strcat_s(g_videoCardDescription, rendererString);

        return true;
}

bool LoadExtensionList()
{
        // Load the OpenGL extensions that this application will be using.

        glAttachShader = (PFNGLATTACHSHADERPROC)glfwGetProcAddress("glAttachShader");
        if(!glAttachShader)
        {
                return false;
        }

        glBindBuffer = (PFNGLBINDBUFFERPROC)glfwGetProcAddress("glBindBuffer");
        if(!glBindBuffer)
        {
                return false;
        }

        glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
        if(!glBindVertexArray)
        {
                return false;
        }

        glBufferData = (PFNGLBUFFERDATAPROC)glfwGetProcAddress("glBufferData");
        if(!glBufferData)
        {
                return false;
        }

        glCompileShader = (PFNGLCOMPILESHADERPROC)glfwGetProcAddress("glCompileShader");
        if(!glCompileShader)
        {
                return false;
        }

        glCreateProgram = (PFNGLCREATEPROGRAMPROC)glfwGetProcAddress("glCreateProgram");
        if(!glCreateProgram)
        {
                return false;
        }

        glCreateShader = (PFNGLCREATESHADERPROC)glfwGetProcAddress("glCreateShader");
        if(!glCreateShader)
        {
                return false;
        }

        glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glfwGetProcAddress("glDeleteBuffers");
        if(!glDeleteBuffers)
        {
                return false;
        }
        
        glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
        if(!glDeleteProgram)
        {
                return false;
        }

        glDeleteShader = (PFNGLDELETESHADERPROC)glfwGetProcAddress("glDeleteShader");
        if(!glDeleteShader)
        {
                return false;
        }

        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress("glDeleteVertexArrays");
        if(!glDeleteVertexArrays)
        {
                return false;
        }

        glDetachShader = (PFNGLDETACHSHADERPROC)glfwGetProcAddress("glDetachShader");
        if(!glDetachShader)
        {
                return false;
        }

        glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glfwGetProcAddress("glEnableVertexAttribArray");
        if(!glEnableVertexAttribArray)
        {
                return false;
        }

        glGenBuffers = (PFNGLGENBUFFERSPROC)glfwGetProcAddress("glGenBuffers");
        if(!glGenBuffers)
        {
                return false;
        }

        glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");
        if(!glGenVertexArrays)
        {
                return false;
        }

        glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)glfwGetProcAddress("glGetAttribLocation");
        if(!glGetAttribLocation)
        {
                return false;
        }

        glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glfwGetProcAddress("glGetProgramInfoLog");
        if(!glGetProgramInfoLog)
        {
                return false;
        }

        glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glfwGetProcAddress("glGetProgramiv");
        if(!glGetProgramiv)
        {
                return false;
        }

        glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glfwGetProcAddress("glGetShaderInfoLog");
        if(!glGetShaderInfoLog)
        {
                return false;
        }

        glGetShaderiv = (PFNGLGETSHADERIVPROC)glfwGetProcAddress("glGetShaderiv");
        if(!glGetShaderiv)
        {
                return false;
        }

        glLinkProgram = (PFNGLLINKPROGRAMPROC)glfwGetProcAddress("glLinkProgram");
        if(!glLinkProgram)
        {
                return false;
        }

        glShaderSource = (PFNGLSHADERSOURCEPROC)glfwGetProcAddress("glShaderSource");
        if(!glShaderSource)
        {
                return false;
        }

        glUseProgram = (PFNGLUSEPROGRAMPROC)glfwGetProcAddress("glUseProgram");
        if(!glUseProgram)
        {
                return false;
        }

        glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glfwGetProcAddress("glVertexAttribPointer");
        if(!glVertexAttribPointer)
        {
                return false;
        }

        glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)glfwGetProcAddress("glBindAttribLocation");
        if(!glBindAttribLocation)
        {
                return false;
        }

        glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glfwGetProcAddress("glGetUniformLocation");
        if(!glGetUniformLocation)
        {
                return false;
        }

        glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glfwGetProcAddress("glUniformMatrix4fv");
        if(!glUniformMatrix4fv)
        {
                return false;
        }

        glActiveTexture = (PFNGLACTIVETEXTUREPROC)glfwGetProcAddress("glActiveTexture");
        if(!glActiveTexture)
        {
                return false;
        }

        glUniform1i = (PFNGLUNIFORM1IPROC)glfwGetProcAddress("glUniform1i");
        if(!glUniform1i)
        {
                return false;
        }

        glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glfwGetProcAddress("glGenerateMipmap");
        if(!glGenerateMipmap)
        {
                return false;
        }

        glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glfwGetProcAddress("glDisableVertexAttribArray");
        if(!glDisableVertexAttribArray)
        {
                return false;
        }

        glUniform3fv = (PFNGLUNIFORM3FVPROC)glfwGetProcAddress("glUniform3fv");
        if(!glUniform3fv)
        {
                return false;
        }

        glUniform4fv = (PFNGLUNIFORM4FVPROC)glfwGetProcAddress("glUniform4fv");
        if(!glUniform4fv)
        {
                return false;
        }

        return true;
}

void GetVideoCardInfo(char* cardName)
{
        strcpy_s(cardName, 128, g_videoCardDescription);
        return;
}

bool InitializeExtensions()
{
        bool result;

        // Initialize the OpenGL extensions needed for this application.  Note that a temporary rendering context was needed to do so.
        result = LoadExtensionList();
        if(!result)
        {
                return false;
        }
        return true;
}

void OutputShaderErrorMessage(HWND hwnd, unsigned int shaderId, const char* shaderFilename)
{
        int logSize, i;
        char* infoLog;
        ofstream fout;
        wchar_t newString[128];
        unsigned int error;
        size_t convertedChars;


        // Get the size of the string containing the information log for the failed shader compilation message.
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

        // Increment the size by one to handle also the null terminator.
        logSize++;

        // Create a char buffer to hold the info log.
        infoLog = new char[logSize];
        if(!infoLog)
        {
                return;
        }

        // Now retrieve the info log.
        glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);

        // Open a file to write the error message to.
        fout.open("shader-error.txt");

        // Write out the error message.
        for(i=0; i<logSize; i++)
        {
                fout << infoLog[i];
        }

        // Close the file.
        fout.close();

        // Convert the shader filename to a wide character string.
        error = mbstowcs_s(&convertedChars, newString, 128, shaderFilename, 128);
        if(error != 0)
        {
                return;
        }

        // Pop a message up on the screen to notify the user to check the text file for compile errors.
        MessageBoxW(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", newString, MB_OK);

        return;
}

void OutputLinkerErrorMessage(HWND hwnd, unsigned int programId)
{
        int logSize, i;
        char* infoLog;
        ofstream fout;


        // Get the size of the string containing the information log for the failed shader compilation message.
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);

        // Increment the size by one to handle also the null terminator.
        logSize++;

        // Create a char buffer to hold the info log.
        infoLog = new char[logSize];
        if(!infoLog)
        {
                return;
        }

        // Now retrieve the info log.
        glGetProgramInfoLog(programId, logSize, NULL, infoLog);

        // Open a file to write the error message to.
        fout.open("linker-error.txt");

        // Write out the error message.
        for(i=0; i<logSize; i++)
        {
                fout << infoLog[i];
        }

        // Close the file.
        fout.close();

        // Pop a message up on the screen to notify the user to check the text file for linker errors.
        MessageBox(hwnd, _T("Error compiling linker.  Check linker-error.txt for message."), _T("Linker Error"), MB_OK);
}

char* LoadShaderSourceFile(const char* filename)
{
        ifstream fin;
        int fileSize;
        char input;
        char* buffer;


        // Open the shader source file.
        fin.open(filename);

        // If it could not open the file then exit.
        if(fin.fail())
        {
                return 0;
        }

        // Initialize the size of the file.
        fileSize = 0;

        // Read the first element of the file.
        fin.get(input);

        // Count the number of elements in the text file.
        while(!fin.eof())
        {
                fileSize++;
                fin.get(input);
        }

        // Close the file for now.
        fin.close();

        // Initialize the buffer to read the shader source file into.
        buffer = new char[fileSize+1];
        if(!buffer)
        {
                return 0;
        }

        // Open the shader source file again.
        fin.open(filename);

        // Read the shader text file into the buffer as a block.
        fin.read(buffer, fileSize);

        // Close the file.
        fin.close();

        // Null terminate the buffer.
        buffer[fileSize] = '\0';

        return buffer;
}

bool InitializeShader(const char* vsFilename, const char* fsFilename)
{
        const char* vertexShaderBuffer;
        const char* fragmentShaderBuffer;
        int status;

        // Load the vertex shader source file into a text buffer.
        vertexShaderBuffer = LoadShaderSourceFile(vsFilename);
        if(!vertexShaderBuffer)
        {
                return false;
        }

        // Load the fragment shader source file into a text buffer.
        fragmentShaderBuffer = LoadShaderSourceFile(fsFilename);
        if(!fragmentShaderBuffer)
        {
                return false;
        }

        // Create a vertex and fragment shader object.
        g_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        g_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Copy the shader source code strings into the vertex and fragment shader objects.
        glShaderSource(g_vertexShader, 1, &vertexShaderBuffer, NULL);
        glShaderSource(g_fragmentShader, 1, &fragmentShaderBuffer, NULL);

        // Release the vertex and fragment shader buffers.
        delete [] vertexShaderBuffer;
        vertexShaderBuffer = 0;

        delete [] fragmentShaderBuffer;
        fragmentShaderBuffer = 0;

        // Compile the shaders.
        glCompileShader(g_vertexShader);
        glCompileShader(g_fragmentShader);

        // Check to see if the vertex shader compiled successfully.
        glGetShaderiv(g_vertexShader, GL_COMPILE_STATUS, &status);
        if(status != 1)
        {
                // If it did not compile then write the syntax error message out to a text file for review.
                //OutputShaderErrorMessage(hwnd, g_vertexShader, vsFilename);
                return false;
        }

        // Check to see if the fragment shader compiled successfully.
        glGetShaderiv(g_fragmentShader, GL_COMPILE_STATUS, &status);
        if(status != 1)
        {
                // If it did not compile then write the syntax error message out to a text file for review.
                //OutputShaderErrorMessage(hwnd, g_fragmentShader, fsFilename);
                return false;
        }

        // Create a shader program object.
        g_shaderProgram = glCreateProgram();

        // Attach the vertex and fragment shader to the program object.
        glAttachShader(g_shaderProgram, g_vertexShader);
        glAttachShader(g_shaderProgram, g_fragmentShader);

        // Bind the shader input variables.
        glBindAttribLocation(g_shaderProgram, 0, "inputPosition");
        glBindAttribLocation(g_shaderProgram, 1, "inputColor");

        // Link the shader program.
        glLinkProgram(g_shaderProgram);

        // Check the status of the link.
        glGetProgramiv(g_shaderProgram, GL_LINK_STATUS, &status);
        if(status != 1)
        {
                // If it did not link then write the syntax error message out to a text file for review.
                //OutputLinkerErrorMessage(hwnd, g_shaderProgram);
                return false;
        }

        return true;
}

void ShutdownShader()
{
        // Detach the vertex and fragment shaders from the program.
        glDetachShader(g_shaderProgram, g_vertexShader);
        glDetachShader(g_shaderProgram, g_fragmentShader);

        // Delete the vertex and fragment shaders.
        glDeleteShader(g_vertexShader);
        glDeleteShader(g_fragmentShader);

        // Delete the shader program.
        glDeleteProgram(g_shaderProgram);
}

bool SetShaderParameters(float* worldMatrix, float* viewMatrix, float* projectionMatrix)
{
        unsigned int location;

        // Set the world matrix in the vertex shader.
        location = glGetUniformLocation(g_shaderProgram, "worldMatrix");
        if(location == -1)
        {
                return false;
        }
        glUniformMatrix4fv(location, 1, false, worldMatrix);

        // Set the view matrix in the vertex shader.
        location = glGetUniformLocation(g_shaderProgram, "viewMatrix");
        if(location == -1)
        {
                return false;
        }
        glUniformMatrix4fv(location, 1, false, viewMatrix);

        // Set the projection matrix in the vertex shader.
        location = glGetUniformLocation(g_shaderProgram, "projectionMatrix");
        if(location == -1)
        {
                return false;
        }
        glUniformMatrix4fv(location, 1, false, projectionMatrix);

        return true;
}

bool InitializeBuffers()
{
        VertexType vertices[] = {
            {{  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }},
            {{  1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }},
            {{ -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }},
            {{ -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 0.0f }},
            {{  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 1.0f }},
            {{  1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f, 1.0f }},
            {{ -1.0f, -1.0f, -1.0f }, { 0.5f, 1.0f, 0.5f }},
            {{ -1.0f, -1.0f,  1.0f }, { 1.0f, 0.5f, 1.0f }},
        };
        uint16_t indices[] = { 1, 2, 3, 3, 2, 6, 6, 7, 3, 3, 0, 1, 0, 3, 7, 7, 6, 4, 4, 6, 5, 0, 7, 4, 1, 0, 4, 1, 4, 5, 2, 1, 5, 2, 5, 6 };

        // Set the number of vertices in the vertex array.
        g_vertexCount = sizeof(vertices) / sizeof(VertexType);

        // Set the number of indices in the index array.
        g_indexCount = sizeof(indices) / sizeof(uint16_t);

        // Allocate an OpenGL vertex array object.
        glGenVertexArrays(1, &g_vertexArrayId);

        // Bind the vertex array object to store all the buffers and vertex attributes we create here.
        glBindVertexArray(g_vertexArrayId);

        // Generate an ID for the vertex buffer.
        glGenBuffers(1, &g_vertexBufferId);

        // Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
        glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, g_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

        // Enable the two vertex array attributes.
        glEnableVertexAttribArray(0);  // Vertex position.
        glEnableVertexAttribArray(1);  // Vertex color.

        // Specify the location and format of the position portion of the vertex buffer.
        glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferId);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);

        // Specify the location and format of the color portion of the vertex buffer.
        glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferId);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (char*)NULL + (3 * sizeof(float)));

        // Generate an ID for the index buffer.
        glGenBuffers(1, &g_indexBufferId);

        // Bind the index buffer and load the index data into it.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_indexCount* sizeof(uint16_t), indices, GL_STATIC_DRAW);

        return true;
}

void ShutdownBuffers()
{
        // Disable the two vertex array attributes.
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Release the vertex buffer.
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &g_vertexBufferId);

        // Release the index buffer.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &g_indexBufferId);

        // Release the vertex array object.
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &g_vertexArrayId);

        return;
}

void RenderBuffers()
{
        // Bind the vertex array object that stored all the information about the vertex and index buffers.
        glBindVertexArray(g_vertexArrayId);

        // Render the vertex buffer using the index buffer.
        glDrawElements(GL_TRIANGLES, g_indexCount, GL_UNSIGNED_SHORT, 0);

        return;
}

void CalculateCameraPosition()
{
    VectorType up, position, lookAt;
    float yaw, pitch, roll;
    float rotationMatrix[9];


    // Setup the vector that points upwards.
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    // Setup the position of the camera in the world.
    position.x = g_positionX;
    position.y = g_positionY;
    position.z = g_positionZ;

    // Setup where the camera is looking by default.
    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = 1.0f;

    // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
    pitch = g_rotationX * 0.0174532925f;
    yaw   = g_rotationY * 0.0174532925f;
    roll  = g_rotationZ * 0.0174532925f;

    // Create the rotation matrix from the yaw, pitch, and roll values.
    MatrixRotationYawPitchRoll(rotationMatrix, yaw, pitch, roll);

    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
    TransformCoord(lookAt, rotationMatrix);
    TransformCoord(up, rotationMatrix);

    // Translate the rotated camera position to the location of the viewer.
    lookAt.x = position.x + lookAt.x;
    lookAt.y = position.y + lookAt.y;
    lookAt.z = position.z + lookAt.z;

    // Finally create the view matrix from the three updated vectors.
    BuildViewMatrix(position, lookAt, up, g_viewMatrix);
}

void Draw(GLFWwindow* window)
{
    static float rotateAngle = 0.0f;

    // Set the color to clear the screen to.
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    // Clear the screen and depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update world matrix to rotate the model
    rotateAngle += PI / 120;
    float rotationMatrixY[16];
    float rotationMatrixZ[16];
    MatrixRotationY(rotationMatrixY, rotateAngle);
    MatrixRotationZ(rotationMatrixZ, rotateAngle);
    MatrixMultiply(g_worldMatrix, rotationMatrixZ, rotationMatrixY);

    // Generate the view matrix based on the camera's position.
    CalculateCameraPosition();

    // Set the color shader as the current shader program and set the matrices that it will use for rendering.
    glUseProgram(g_shaderProgram);
    SetShaderParameters(g_worldMatrix, g_viewMatrix, g_projectionMatrix);

    // Render the model using the color shader.
    RenderBuffers();

    // Present the back buffer to the screen since rendering is complete.
    glfwSwapBuffers(window);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    GLFWwindow* window = glfwCreateWindow(900, 540,"HelloGLFW", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    
    InitializeExtensions();
    InitializeOpenGL(900, 540, SCREEN_DEPTH, SCREEN_NEAR, true);
    InitializeShader(VS_SHADER_SOURCE_FILE, PS_SHADER_SOURCE_FILE);
    InitializeBuffers();

    while (!glfwWindowShouldClose(window))
    {
        Draw(window);
        int e = glGetError();
        glfwPollEvents();
    }

    ShutdownBuffers();
    ShutdownShader();
    glfwDestroyWindow(window);
    return 0;
}

