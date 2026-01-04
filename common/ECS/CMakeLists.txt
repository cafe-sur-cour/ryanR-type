set(ECS_LIB_NAME ECS)

add_library(${ECS_LIB_NAME} STATIC)

find_path(SOL2_INCLUDE_DIRS "sol/sol.hpp")
find_package(Lua REQUIRED)

target_include_directories(${ECS_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
    PUBLIC ${SOL2_INCLUDE_DIRS}
    PUBLIC ${LUA_INCLUDE_DIR}
)

add_subdirectory(entity)
add_subdirectory(view)

set_target_properties(${ECS_LIB_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${ECS_LIB_NAME}
    PUBLIC ENTITY_PREFAB_MANAGER
    PUBLIC SPATIAL_GRID
    PUBLIC COLLISION_RULES
    PRIVATE ${LUA_LIBRARIES}
)

target_include_directories(${ECS_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)
