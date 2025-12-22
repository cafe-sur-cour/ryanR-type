add_library(AIMovementSystem SHARED
    AIMovementSystem.hpp
    AIMovementSystem.cpp
)

target_link_libraries(AIMovementSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(AIMovementSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)

add_library(AIShootingSystem SHARED
    AIShootingSystem.hpp
    AIShootingSystem.cpp
)

target_link_libraries(AIShootingSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(AIShootingSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)
