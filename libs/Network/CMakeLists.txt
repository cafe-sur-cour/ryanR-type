set(NETWORK_SERVER_LIB_NAME NetworkServer)
set(NETWORK_CLIENT_LIB_NAME NetworkClient)

if(WIN32)
    add_subdirectory(Windows)
elseif(UNIX)
    add_subdirectory(Unix)
endif()
