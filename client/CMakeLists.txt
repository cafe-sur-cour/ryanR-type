set(CLIENT_BIN_NAME r-type_client)

add_executable(${CLIENT_BIN_NAME}
    main.cpp
    Utils.cpp
    Utils.hpp
    ClientNetwork.cpp
    ClientLibLoading.cpp
    ClientNetwork.hpp
    ClientReceivedPacket.cpp
    ClientSentPacket.cpp
    ClientGameStateConversions.cpp
    Core.cpp
    SettingsConfig.cpp
    SettingsConfig.hpp
    SettingsManager.cpp
    SettingsManager.hpp
    DeathAnimationSpawner.cpp
    DeathAnimationSpawner.hpp
)

add_subdirectory(packet)

add_dependencies(${CLIENT_BIN_NAME} generate_embedded_assets)

target_link_libraries(${CLIENT_BIN_NAME}
    PUBLIC
        ECS
        COLLISION_RULES
        ERROR
        DLLoader
        SIGNAL
        PREFAB
        PARSER
        UI
        DEBUG
        InputNormalizer
        NETWORK_INTERPOLATION
        AsioNetwork
        nlohmann_json::nlohmann_json
        Packet
        Client::PacketHandlers
)

target_include_directories(${CLIENT_BIN_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/common)
target_include_directories(${CLIENT_BIN_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${CLIENT_BIN_NAME}
    PUBLIC R_TYPE_CLIENT
)

if(USE_PCH)
    target_precompile_headers(${CLIENT_BIN_NAME} PRIVATE ${PCH_HEADERS})
endif()

add_subdirectory(initResourcesManager)
add_subdirectory(gsm)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(input)
add_subdirectory(ui)
add_subdirectory(interpolation)

add_custom_command(TARGET ${CLIENT_BIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CLIENT_BIN_NAME}> ${CMAKE_SOURCE_DIR}/
    COMMENT "Copying ${CLIENT_BIN_NAME} to root directory"
)

if(WIN32)
    # Copy vcpkg DLLs (and PDBs only in Debug mode)
    add_custom_command(TARGET ${CLIENT_BIN_NAME} POST_BUILD
        COMMAND powershell -Command "Get-ChildItem -Path '${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/*.dll' | ForEach-Object { Copy-Item $_.FullName -Destination '${CMAKE_SOURCE_DIR}/' -Force }"
        COMMENT "Copying vcpkg DLLs to root directory"
    )
    add_custom_command(TARGET ${CLIENT_BIN_NAME} POST_BUILD
        COMMAND powershell -Command "if ('$<CONFIG>' -eq 'Debug') { Get-ChildItem -Path '${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/*.pdb' -ErrorAction SilentlyContinue | ForEach-Object { Copy-Item $_.FullName -Destination '${CMAKE_SOURCE_DIR}/' -Force } }"
        COMMENT "Copying vcpkg PDBs to root directory (Debug only)"
    )
endif()

if(USE_UNITY_BUILD)
    set_target_properties(${CLIENT_BIN_NAME} PROPERTIES UNITY_BUILD ON)
endif()