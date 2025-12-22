add_library(MovementSystem SHARED
    MovementSystem.hpp
    MovementSystem.cpp
)

target_link_libraries(MovementSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(MovementSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)

add_library(InputToVelocitySystem SHARED
    InputToVelocitySystem.hpp
    InputToVelocitySystem.cpp
)

target_link_libraries(InputToVelocitySystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(InputToVelocitySystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)