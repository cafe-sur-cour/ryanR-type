set(CLIENT_BIN_NAME r-type_client)

add_executable(${CLIENT_BIN_NAME}
    main.cpp
)

find_package(SFML COMPONENTS Graphics Window System Audio REQUIRED)

target_link_libraries(${CLIENT_BIN_NAME}
    PRIVATE
        ECS
        Network
        Packet
        Buffer
        DLLoader
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

add_subdirectory(gsm)
add_subdirectory(graphicals)
