include_directories("${PROJECT_SOURCE_DIR}/3rdparty/glfw3/include")
link_directories("${PROJECT_SOURCE_DIR}/3rdparty/glfw3")

add_executable(HelloGLFW
HelloGLFW.cpp
)

target_link_libraries(HelloGLFW
opengl32.lib
glfw3.lib
)

set_target_properties(HelloGLFW PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})