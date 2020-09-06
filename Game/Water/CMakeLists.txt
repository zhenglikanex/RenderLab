add_executable(WaterGame
WaterGameConfig.cpp
WaterGameLogic.hpp
WaterGameLogic.cpp
)

target_link_libraries(WaterGame
EngineOpenGL
MyPhysics
)

SET_PROPERTY(TARGET WaterGame PROPERTY FOLDER "Game")

set_target_properties(WaterGame PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)