add_subdirectory(Network)
add_subdirectory(Buffer)
add_subdirectory(Packet)

if(BUILD_CLIENT)
    add_subdirectory(Multimedia)
endif()
