add_executable(BilliardGame
BilliardGameConfig.cpp
BilliardGameLogic.hpp
BilliardGameLogic.cpp
)

target_link_libraries(BilliardGame
EngineOpenGL)

SET_PROPERTY(TARGET BilliardGame PROPERTY FOLDER "Game")

#set_target_properties(BilliardGame PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)