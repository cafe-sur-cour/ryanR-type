add_library(ServerMovementInputSystem STATIC
    input/ServerMovementInputSystem.cpp
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
    input/ServerShootInputSystem.cpp
)

target_link_libraries(ServerShootInputSystem
    ECS
    ${SERVER_LIB_NAME}
)

target_include_directories(ServerShootInputSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/server
)

add_library(EndOfMapDetectionSystem STATIC
    gameEnd/EndOfMapDetectionSystem.cpp
)

target_link_libraries(EndOfMapDetectionSystem
    ECS
    ${SERVER_LIB_NAME}
)

target_include_directories(EndOfMapDetectionSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/server
)