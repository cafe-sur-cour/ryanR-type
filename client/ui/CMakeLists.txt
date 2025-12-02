add_library(UI STATIC)

target_include_directories(UI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/common
        ${CMAKE_SOURCE_DIR}/libs
)

target_link_libraries(UI
    PUBLIC
        ECS
        Multimedia
)

add_subdirectory(core)
add_subdirectory(elements)
add_subdirectory(navigation)
add_subdirectory(manager)
