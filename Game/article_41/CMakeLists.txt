add_executable(Article41
article_41_config.cpp
article_41_game_logic.hpp
article_41_game_logic.cpp
)

target_link_libraries(Article41
EngineOpenGL
MyPhysics
)

SET_PROPERTY(TARGET Article41 PROPERTY FOLDER "Game")

set_target_properties(Article41 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)