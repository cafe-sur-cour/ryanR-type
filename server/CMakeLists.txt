set(SERVER_LIB_NAME r-type_server_lib)

add_library(${SERVER_LIB_NAME}
    ServerConfig.hpp
    ServerConfig.cpp
    Server.hpp
    Server.cpp
    Lobby.cpp
    Lobby.hpp
    ServerLibsLoading.cpp
    ServerReceivePacket.cpp
    ServerSentPacket.cpp
    ECSConversions.cpp
    Utils.cpp
    Utils.hpp
    LobbyStruct.hpp
    ../common/utils/Encryption.cpp
    ../common/utils/SecureJsonManager.cpp
)

target_link_libraries(${SERVER_LIB_NAME}
    PUBLIC
        ECS
        COLLISION_RULES
        ERROR
        SIGNAL
        DLLoader
        NetworkServer
        NetworkClient
        DEBUG
        Packet
        nlohmann_json::nlohmann_json
        PARSER
        PREFAB
        InputNormalizer
        ServerMovementInputSystem
        ServerShootInputSystem
        ServerForceInputSystem
        EndOfMapDetectionSystem
        Server::PacketHandlers
)

target_include_directories(${SERVER_LIB_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/common)
target_include_directories(${SERVER_LIB_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(USE_PCH)
    target_precompile_headers(${SERVER_LIB_NAME} PRIVATE ${PCH_HEADERS})
endif()

add_subdirectory(packet)
add_subdirectory(gsm)
add_subdirectory(initResourcesManager)
add_subdirectory(systems)
add_subdirectory(deltaTracker)

set(SERVER_BIN_NAME r-type_server)

add_executable(${SERVER_BIN_NAME}
    main.cpp
)

target_link_libraries(${SERVER_BIN_NAME}
    PRIVATE
        ${SERVER_LIB_NAME}
)

add_custom_command(TARGET ${SERVER_BIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SERVER_BIN_NAME}> ${CMAKE_SOURCE_DIR}/
    COMMENT "Copying ${SERVER_BIN_NAME} to root directory"
)

if(WIN32)
    # Copy vcpkg DLLs (and PDBs only in Debug mode)
    add_custom_command(TARGET ${SERVER_BIN_NAME} POST_BUILD
        COMMAND powershell -Command "Get-ChildItem -Path '${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/*.dll' | ForEach-Object { Copy-Item $_.FullName -Destination '${CMAKE_SOURCE_DIR}/' -Force }"
        COMMENT "Copying vcpkg DLLs to root directory"
    )
    add_custom_command(TARGET ${SERVER_BIN_NAME} POST_BUILD
        COMMAND powershell -Command "if ('$<CONFIG>' -eq 'Debug') { Get-ChildItem -Path '${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/*.pdb' -ErrorAction SilentlyContinue | ForEach-Object { Copy-Item $_.FullName -Destination '${CMAKE_SOURCE_DIR}/' -Force } }"
        COMMENT "Copying vcpkg PDBs to root directory (Debug only)"
    )
endif()
