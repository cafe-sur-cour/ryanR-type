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