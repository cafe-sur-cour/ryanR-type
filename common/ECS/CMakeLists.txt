set(ECS_LIB_NAME ECS)

add_library(${ECS_LIB_NAME} STATIC)

target_include_directories(${ECS_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

add_subdirectory(component)
add_subdirectory(entity)
add_subdirectory(resourceManager)
add_subdirectory(system)
add_subdirectory(view)
