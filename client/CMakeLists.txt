set(CLIENT_BIN_NAME r-type_client)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

add_executable(generate_assets ../scripts/generate_embedded_assets.cpp)
target_include_directories(generate_assets PRIVATE ${CMAKE_SOURCE_DIR}/client/graphicals/assets)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp ${CMAKE_BINARY_DIR}/EmbeddedAssets.hpp
    COMMAND generate_assets ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/EmbeddedAssets
    DEPENDS generate_assets
)

set_source_files_properties(${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_BINARY_DIR}/EmbeddedAssets.hpp PROPERTIES GENERATED TRUE)

add_custom_target(generate_embedded_assets DEPENDS ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp ${CMAKE_BINARY_DIR}/EmbeddedAssets.hpp)

add_executable(${CLIENT_BIN_NAME}
    main.cpp
    ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp
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
