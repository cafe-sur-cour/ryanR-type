set(UI_SOURCES
    UIElement.cpp
    UIElement.hpp
    Button.cpp
    Button.hpp
)

add_library(UI ${UI_SOURCES})

target_link_libraries(UI
    PUBLIC
        ECS
        Multimedia
)

target_include_directories(UI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/common
        ${CMAKE_SOURCE_DIR}/libs
)
