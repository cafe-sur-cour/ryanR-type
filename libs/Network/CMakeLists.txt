set(NETWORK_LIB_NAME Network)

add_library(${NETWORK_LIB_NAME} SHARED
    INetwork.hpp
    ANetwork.hpp
)

target_include_directories(${NETWORK_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${NETWORK_LIB_NAME}
    PRIVATE Packet
)

if(WIN32)
    add_subdirectory(WindowsNetwork)
elseif(UNIX)
    add_subdirectory(LinuxNetwork)
endif()
