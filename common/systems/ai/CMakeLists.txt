target_sources(${ECS_LIB_NAME} PRIVATE
    AIMovementSystem.cpp
    AIShootingSystem.cpp
)

target_sources(${ECS_LIB_NAME} PUBLIC
    AIMovementSystem.hpp
    AIShootingSystem.hpp
)
