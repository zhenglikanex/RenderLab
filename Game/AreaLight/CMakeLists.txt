add_executable(AreaLightGame
AreaLightGameConfig.cpp
AreaLightGameLogic.hpp
AreaLightGameLogic.cpp
)

target_link_libraries(AreaLightGame
EngineOpenGL
MyPhysics
)

SET_PROPERTY(TARGET AreaLightGame PROPERTY FOLDER "Game")

set_target_properties(AreaLightGame PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)