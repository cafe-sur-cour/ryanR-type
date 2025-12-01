add_subdirectory(scenes)

target_sources(${SERVER_LIB_NAME} PUBLIC
    IGameState.hpp
    AGameState.hpp
    AGameState.cpp
)
