add_library(EndOfMapDetectionSystem SHARED
    EndOfMapDetectionSystem.cpp
)

target_link_libraries(EndOfMapDetectionSystem
    ECS
    ServerInputLib
)

target_include_directories(EndOfMapDetectionSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/server
)

set_target_properties(EndOfMapDetectionSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)
