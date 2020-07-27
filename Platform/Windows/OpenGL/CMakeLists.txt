add_executable(EngineOpenGL
${PROJECT_SOURCE_DIR}/3rdparty/glad/src/glad_wgl.c
../WindowsApplication.hpp
../WindowsApplication.cpp
OpenGLApplication.hpp
OpenGLApplication.cpp)

target_link_libraries(EngineOpenGL Common Utils OpenGLRHI opengl32)
target_include_directories(EngineOpenGL PRIVATE "${PROJECT_SOURCE_DIR}/3rdparty/glad/include")

SET_PROPERTY(TARGET EngineOpenGL PROPERTY FOLDER "Platform")

