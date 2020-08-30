add_executable(ShadowMapGame
ShadowMapGameConfig.cpp
ShadowMapGameLogic.hpp
ShadowMapGameLogic.cpp
)

target_link_libraries(ShadowMapGame
EngineOpenGL
MyPhysics
)

SET_PROPERTY(TARGET ShadowMapGame PROPERTY FOLDER "Game")

set_target_properties(ShadowMapGame PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)