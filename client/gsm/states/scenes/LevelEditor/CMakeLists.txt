target_sources(${CLIENT_BIN_NAME} PRIVATE
    LevelEditorState.hpp
    LevelEditorState.cpp
    LevelEditorRender.cpp
    LevelEditorHistory.cpp
    LevelEditorUtils.cpp
    LevelEditorObstacles.cpp
    LevelEditorPowerup.cpp
    LevelEditorWaves.cpp
)

include(UI/CMakeLists.txt)
