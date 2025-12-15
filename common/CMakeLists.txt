set(ECS_LIB_NAME ECS)
set(ERROR_LIB ERROR)

add_subdirectory(DLLoader)
add_subdirectory(ECS)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(types)
add_subdirectory(Error)
add_subdirectory(Signal)
add_subdirectory(Prefab)
add_subdirectory(resourceManager)
add_subdirectory(InputMapping)
add_subdirectory(Parser)
add_subdirectory(CollisionRules)
add_subdirectory(SpatialGrid)
add_subdirectory(packet)

set(DEBUG_LIB_NAME DEBUG)

add_library(${DEBUG_LIB_NAME} STATIC)

target_include_directories(${DEBUG_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${DEBUG_LIB_NAME} PUBLIC
    debug.cpp
    debug.hpp
)
