set(NETWORK_LIB_NAME Network)

if(WIN32)
    add_subdirectory(Windows)
elseif(UNIX)
    add_subdirectory(Unix)
endif()
