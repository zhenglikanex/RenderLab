add_executable(SunLightGame
SunLightGameConfig.cpp
SunLightGameLogic.hpp
SunLightGameLogic.cpp
)

target_link_libraries(SunLightGame
EngineOpenGL
MyPhysics
)

SET_PROPERTY(TARGET SunLightGame PROPERTY FOLDER "Game")

set_target_properties(SunLightGame PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)