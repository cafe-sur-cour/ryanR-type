set(VIEW_LIB_NAME view)

add_library(${VIEW_LIB_NAME} STATIC
    View.cpp
)

target_include_directories(${VIEW_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../entity/registry
)

target_link_libraries(${VIEW_LIB_NAME}
    PRIVATE ECS
)
