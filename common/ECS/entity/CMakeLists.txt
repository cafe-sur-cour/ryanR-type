target_sources(${ECS_LIB_NAME} PUBLIC
    Entity.hpp
    EntityCreationContext.hpp
)

add_subdirectory(componentArray)
add_subdirectory(registry)
add_subdirectory(factory)
