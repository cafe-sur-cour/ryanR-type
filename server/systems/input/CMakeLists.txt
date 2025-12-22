add_library(ServerMovementInputSystem STATIC
    ServerMovementInputSystem.cpp
)

target_link_libraries(ServerMovementInputSystem
    InputNormalizer
    ECS
    ${SERVER_LIB_NAME}
)

target_include_directories(ServerMovementInputSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/server
)

add_library(ServerShootInputSystem STATIC
    ServerShootInputSystem.cpp
)

target_link_libraries(ServerShootInputSystem
    ECS
    ${SERVER_LIB_NAME}
)

target_include_directories(ServerShootInputSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/server
)