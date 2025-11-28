add_library(${NETWORK_LIB_NAME} SHARED
    UnixNetwork.cpp
)

target_include_directories(${NETWORK_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${NETWORK_LIB_NAME}
    PUBLIC ASIO_STANDALONE
)

# Link asio for networking functionality
target_link_libraries(${NETWORK_LIB_NAME}
    PUBLIC asio::asio
)

add_custom_command(TARGET ${NETWORK_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NETWORK_LIB_NAME}> ${CMAKE_SOURCE_DIR}/libraries/
)
