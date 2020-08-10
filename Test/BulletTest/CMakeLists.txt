add_executable(BulletTest
BulletTest.cpp
)

target_link_libraries(BulletTest
Common
)

set_target_properties(BulletTest PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})