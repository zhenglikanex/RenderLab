SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

add_executable(HelloOpenGL
HelloOpenGL.cpp
)

target_link_libraries(HelloOpenGL
opengl32.lib
)

set_target_properties(HelloOpenGL PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})