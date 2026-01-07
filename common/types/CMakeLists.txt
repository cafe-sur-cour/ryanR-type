add_library(TYPES STATIC
    Vector2f.hpp
    Vector2f.cpp
    FRect.hpp
    FRect.cpp
    OrientedRect.hpp
    OrientedRect.cpp
    Chrono.hpp
    Chrono.cpp
)

set_target_properties(TYPES PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(TYPES PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
