add_subdirectory(base)
add_subdirectory(focusable)

target_sources(UI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Text.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Text.hpp
)
