find_package(OpenGL REQUIRED)

add_library(PlatformInterface
GLFWApplication.hpp
GLFWApplication.cpp)

target_link_directories(PlatformInterface PUBLIC "${PROJECT_SOURCE_DIR}/3rdparty/glfw3")

target_include_directories(PlatformInterface PRIVATE "${PROJECT_SOURCE_DIR}/3rdparty/glfw3/include")
target_include_directories(PlatformInterface PRIVATE "${PROJECT_SOURCE_DIR}/3rdparty/glad/include")

target_link_libraries(PlatformInterface Common Utils glfw3 ${OPENGL_LIBRARIES})

SET_PROPERTY(TARGET PlatformInterface PROPERTY FOLDER "Platform")

add_library(EngineOpenGL
OpenGLApplication.cpp)

target_include_directories(EngineOpenGL PRIVATE "${PROJECT_SOURCE_DIR}/3rdparty/glfw3/include")

target_link_libraries(EngineOpenGL
PlatformInterface
OpenGLRHI
)

SET_PROPERTY(TARGET EngineOpenGL PROPERTY FOLDER "Platform")