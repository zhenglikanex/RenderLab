add_executable(EngineOpenGLInterface
${PROJECT_SOURCE_DIR}/3rdparty/glad/src/glad_wgl.c
../WindowsApplication.hpp
../WindowsApplication.cpp
OpenGLApplication.hpp
OpenGLApplication.cpp)

target_link_libraries(EngineOpenGLInterface Common Utils OpenGLRHI opengl32)
target_include_directories(EngineOpenGLInterface PRIVATE "${PROJECT_SOURCE_DIR}/3rdparty/glad/include")

SET_PROPERTY(TARGET EngineOpenGLInterface PROPERTY FOLDER "Platform")

