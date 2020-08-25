add_executable(SpotLightGame
SpotLightGameConfig.cpp
SpotLightGameLogic.hpp
SpotLightGameLogic.cpp
)

target_link_libraries(SpotLightGame
EngineOpenGL
MyPhysics
)

SET_PROPERTY(TARGET SpotLightGame PROPERTY FOLDER "Game")

#set_target_properties(BilliardGame PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)