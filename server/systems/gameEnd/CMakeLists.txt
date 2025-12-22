add_library(EndOfMapDetectionSystem STATIC
    EndOfMapDetectionSystem.cpp
)

target_link_libraries(EndOfMapDetectionSystem
    ECS
    ${SERVER_LIB_NAME}
)

target_include_directories(EndOfMapDetectionSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/server
)