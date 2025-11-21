set(CLIENT_BIN_NAME r-type_client)

add_executable(${CLIENT_BIN_NAME}
    main.cpp
)

find_package(SFML COMPONENTS graphics window system audio REQUIRED)

target_link_libraries(${CLIENT_BIN_NAME}
    PRIVATE
        ECS
        Network
        Packet
        Buffer
        DLLoader
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

add_subdirectory(gsm)
add_subdirectory(graphicals)
