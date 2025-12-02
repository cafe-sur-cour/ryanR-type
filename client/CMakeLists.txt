set(CLIENT_BIN_NAME r-type_client)

add_executable(${CLIENT_BIN_NAME}
    main.cpp
    Utils.cpp
    Utils.hpp
    ClientNetwork.cpp
    ClientNetwork.hpp
    Core.cpp
)

add_dependencies(${CLIENT_BIN_NAME} generate_embedded_assets)

target_link_libraries(${CLIENT_BIN_NAME}
    PUBLIC
        ECS
        DLLoader
        SIGNAL
        PREFAB
        PARSER
        nlohmann_json::nlohmann_json
        UI
)

add_subdirectory(initResourcesManager)
add_subdirectory(gsm)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(input)
add_subdirectory(ui)

add_custom_command(TARGET ${CLIENT_BIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CLIENT_BIN_NAME}> ${CMAKE_SOURCE_DIR}/
    COMMENT "Copying ${CLIENT_BIN_NAME} to root directory"
)
