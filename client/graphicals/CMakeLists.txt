set(GRAPHICAL_LIB_NAME Graphical)

add_library(${GRAPHICAL_LIB_NAME} STATIC)

target_sources(${GRAPHICAL_LIB_NAME} PRIVATE
    SfmlWindow.cpp
    SfmlEvent.cpp
    SfmlKeyMappings.cpp
    TextureManager.cpp
    FontManager.cpp
    assets/AssetManager.cpp
)

target_include_directories(${GRAPHICAL_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/common/ECS/resourceManager
    PRIVATE ${CMAKE_BINARY_DIR}
)

find_package(SFML COMPONENTS Graphics Window System REQUIRED)

target_link_libraries(${GRAPHICAL_LIB_NAME}
    PUBLIC
        ECS
        SFML::Graphics
        SFML::Window
        SFML::System
)

add_dependencies(${GRAPHICAL_LIB_NAME} generate_embedded_assets)

target_sources(${CLIENT_BIN_NAME} PRIVATE
    IEvent.hpp
    EventTypes.hpp
    SfmlKeyMappings.hpp
    SfmlWindow.hpp
    SfmlEvent.hpp
    IWindow.hpp
    TextureManager.hpp
    FontManager.hpp
    assets/AssetManager.hpp
)
