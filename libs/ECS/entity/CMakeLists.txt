target_sources(${ECS_LIB_NAME} PUBLIC
    IEntity.hpp
    AEntity.hpp
    AEntity.cpp
)

add_subdirectory(componentArray)
add_subdirectory(registry)
