set(CLIENT_BIN_NAME r-type_client)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

execute_process(
    COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/generate_embedded_assets.py ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/EmbeddedAssets
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(${CLIENT_BIN_NAME}
    main.cpp
    ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp
)

find_package(SFML COMPONENTS Graphics Window System Audio REQUIRED)

target_link_libraries(${CLIENT_BIN_NAME}
    PUBLIC
        ECS
        DLLoader
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
