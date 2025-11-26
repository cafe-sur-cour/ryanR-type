set(GRAPHICAL_LIB_NAME Graphical)

add_library(${GRAPHICAL_LIB_NAME} STATIC)

target_sources(${GRAPHICAL_LIB_NAME} PRIVATE
    SfmlWindow.cpp
    SfmlEvent.cpp
    SfmlKeyMappings.cpp
)

target_include_directories(${GRAPHICAL_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/common/ECS/resourceManager
)

find_package(SFML COMPONENTS Graphics Window System REQUIRED)

target_link_libraries(${GRAPHICAL_LIB_NAME}
    PUBLIC
        ECS
        SFML::Graphics
        SFML::Window
        SFML::System
)

target_sources(${CLIENT_BIN_NAME} PRIVATE
    IEvent.hpp
    EventTypes.hpp
    SfmlKeyMappings.hpp
    SfmlWindow.hpp
    SfmlEvent.hpp
)
