add_subdirectory(base)
add_subdirectory(focusable)

target_sources(UI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Text.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Text.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Background.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Background.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Box.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Box.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Panel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Panel.hpp
)
