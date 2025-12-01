add_library(${NETWORK_SERVER_LIB_NAME} SHARED
    ServerNetwork.cpp
)

target_include_directories(${NETWORK_SERVER_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${NETWORK_SERVER_LIB_NAME}
    PUBLIC _WIN32_WINNT=0x0A00
    PUBLIC WIN32_LEAN_AND_MEAN
    PRIVATE NETWORK_EXPORTS
)

if(WIN32)
    set_target_properties(${NETWORK_SERVER_LIB_NAME} PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()

target_compile_definitions(${NETWORK_SERVER_LIB_NAME}
    PUBLIC ASIO_STANDALONE
)

# Link asio for networking functionality
target_link_libraries(${NETWORK_SERVER_LIB_NAME}
    PUBLIC asio::asio
)

add_custom_command(TARGET ${NETWORK_SERVER_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NETWORK_SERVER_LIB_NAME}> ${CMAKE_SOURCE_DIR}/libraries/
)




add_library(${NETWORK_CLIENT_LIB_NAME} SHARED
    ClientNetwork.cpp
)

target_include_directories(${NETWORK_CLIENT_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${NETWORK_CLIENT_LIB_NAME}
    PUBLIC _WIN32_WINNT=0x0A00
    PUBLIC WIN32_LEAN_AND_MEAN
    PRIVATE NETWORK_EXPORTS
)

if(WIN32)
    set_target_properties(${NETWORK_CLIENT_LIB_NAME} PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()

target_compile_definitions(${NETWORK_CLIENT_LIB_NAME}
    PUBLIC ASIO_STANDALONE
)

# Link asio for networking functionality
target_link_libraries(${NETWORK_CLIENT_LIB_NAME}
    PUBLIC asio::asio
)

add_custom_command(TARGET ${NETWORK_CLIENT_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NETWORK_CLIENT_LIB_NAME}> ${CMAKE_SOURCE_DIR}/libraries/
)
