# Find ASIO library (confiné à ce module)
find_package(asio CONFIG REQUIRED)

add_library(${NETWORK_SERVER_LIB_NAME} SHARED
    ../ANetwork.cpp
    ../Asio/EventLoop.cpp
    ../Asio/Endpoint.cpp
    ../Asio/ErrorCode.cpp
    ../Asio/Socket.cpp
    ../Asio/Resolver.cpp
    ../Asio/Address.cpp
    ServerNetwork.cpp
)

target_include_directories(${NETWORK_SERVER_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

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
    ../ANetwork.cpp
    ../Asio/EventLoop.cpp
    ../Asio/Endpoint.cpp
    ../Asio/ErrorCode.cpp
    ../Asio/Socket.cpp
    ../Asio/Resolver.cpp
    ../Asio/Address.cpp
    ClientNetwork.cpp
)

target_include_directories(${NETWORK_CLIENT_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${NETWORK_CLIENT_LIB_NAME}
    PUBLIC ASIO_STANDALONE
)

# Disable sign-conversion warnings for ASIO library
if(APPLE OR UNIX)
    target_compile_options(${NETWORK_CLIENT_LIB_NAME} PRIVATE 
        -Wno-sign-conversion 
        -Wno-implicit-int-conversion 
        -Wno-shorten-64-to-32 
        -Wno-shadow
    )
endif()

# Link asio for networking functionality
target_link_libraries(${NETWORK_CLIENT_LIB_NAME}
    PUBLIC asio::asio
)

add_custom_command(TARGET ${NETWORK_CLIENT_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NETWORK_CLIENT_LIB_NAME}> ${CMAKE_SOURCE_DIR}/libraries/
)
