add_executable(GlfwEngineOpenGL
GLFWApplication.hpp
GLFWApplication.cpp)

target_link_directories(GlfwEngineOpenGL PRIVATE "${PROJECT_SOURCE_DIR}/3rdparty/glfw3")

target_include_directories(GlfwEngineOpenGL PRIVATE "${PROJECT_SOURCE_DIR}/3rdparty/glfw3/include")
#target_include_directories(GlfwEngineOpenGL PRIVATE "${PROJECT_SOURCE_DIR}/3rdparty/glad/include")

target_link_libraries(GlfwEngineOpenGL ${OPENGEX_LIBRARY} ${OPENDDL_LIBRARY} Common Utils OpenGLRHI opengl32 glfw3)

set_target_properties(GlfwEngineOpenGL PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)

SET_PROPERTY(TARGET GlfwEngineOpenGL PROPERTY FOLDER "Platform")