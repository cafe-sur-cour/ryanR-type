set(CLIENT_BIN_NAME r-type_client)

add_executable(${CLIENT_BIN_NAME}
    main.cpp
    Utils.cpp
    Utils.hpp
    ClientNetwork.cpp
    ClientNetwork.hpp
    Core.cpp
    MouseInputHandler.cpp
    MouseInputHandler.hpp
)

add_dependencies(${CLIENT_BIN_NAME} generate_embedded_assets)

target_link_libraries(${CLIENT_BIN_NAME}
    PUBLIC
        ECS
        DLLoader
        SIGNAL
        PREFAB
        PARSER
        UI
        asio::asio
        nlohmann_json::nlohmann_json
)

target_compile_definitions(${CLIENT_BIN_NAME}
    PUBLIC ASIO_STANDALONE
)

add_subdirectory(initRessourcesManager)
add_subdirectory(gsm)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(ui)

add_custom_command(TARGET ${CLIENT_BIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CLIENT_BIN_NAME}> ${CMAKE_SOURCE_DIR}/
    COMMENT "Copying ${CLIENT_BIN_NAME} to root directory"
)
