add_library(ServerMovementInputSystem SHARED
    ServerMovementInputSystem.cpp
)

target_link_libraries(ServerMovementInputSystem
    InputNormalizer
    ECS
    ServerInputLib
)

target_include_directories(ServerMovementInputSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/server
)

set_target_properties(ServerMovementInputSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)

add_library(ServerShootInputSystem SHARED
    ServerShootInputSystem.cpp
)

target_link_libraries(ServerShootInputSystem
    ECS
    ServerInputLib
)

target_include_directories(ServerShootInputSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/server
)

set_target_properties(ServerShootInputSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)
