set(NETWORK_LIB_NAME Network)

add_library(${NETWORK_LIB_NAME} SHARED
    INetwork.hpp
    ANetwork.hpp
)

target_include_directories(${NETWORK_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${NETWORK_LIB_NAME}
    PUBLIC ASIO_STANDALONE
)

if(WIN32)
    target_compile_definitions(${NETWORK_LIB_NAME}
        PUBLIC _WIN32_WINNT=0x0601
        PUBLIC WIN32_LEAN_AND_MEAN
    )
endif()

# Link asio for networking functionality
target_link_libraries(${NETWORK_LIB_NAME}
    PUBLIC asio::asio
)

if(WIN32)
    add_subdirectory(WindowsNetwork)
elseif(UNIX)
    add_subdirectory(UnixNetwork)
endif()

add_custom_command(TARGET ${NETWORK_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NETWORK_LIB_NAME}> ${CMAKE_SOURCE_DIR}/libraries/
)
