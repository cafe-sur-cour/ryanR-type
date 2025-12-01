set(NETWORK_LIB_NAME Network)

if(WIN32)
    add_subdirectory(WindowsNetwork)
elseif(UNIX)
    add_subdirectory(UnixNetwork)
endif()
