set(CLIENT_BIN_NAME r-type_client)

add_executable(${CLIENT_BIN_NAME}
    main.cpp
    Utils.cpp
    Utils.hpp
    ClientNetwork.cpp
    ClientNetwork.hpp
    Core.cpp
)

find_package(SFML COMPONENTS Graphics Window System Audio REQUIRED)

target_link_libraries(${CLIENT_BIN_NAME}
    PUBLIC
        ECS
        DLLoader
        SIGNAL
        Graphical
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

add_subdirectory(initRessourcesManager)
add_subdirectory(gsm)
add_subdirectory(graphicals)

add_custom_command(TARGET ${CLIENT_BIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CLIENT_BIN_NAME}> ${CMAKE_SOURCE_DIR}/
    COMMENT "Copying ${CLIENT_BIN_NAME} to root directory"
)
