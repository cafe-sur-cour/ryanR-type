target_sources(${ECS_LIB_NAME} PUBLIC
    SystemLoader.hpp
    SystemLoader.cpp
)

add_subdirectory(base)
add_subdirectory(movement)
add_subdirectory(shooting)
add_subdirectory(interactions)
add_subdirectory(systemManager)
add_subdirectory(lifetime)
add_subdirectory(health)
add_subdirectory(death)
add_subdirectory(score)
add_subdirectory(ai)
add_subdirectory(spawn)
add_subdirectory(input)
add_subdirectory(bounds)
