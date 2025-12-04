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
    Core.cpp
)

add_dependencies(${CLIENT_BIN_NAME} generate_embedded_assets)

target_link_libraries(${CLIENT_BIN_NAME}
    PUBLIC
        ECS
        ERROR
        DLLoader
        SIGNAL
        PREFAB
        PARSER
        UI
        DEBUG
        asio::asio
        nlohmann_json::nlohmann_json
)

target_compile_definitions(${CLIENT_BIN_NAME}
    PUBLIC ASIO_STANDALONE
    PUBLIC R_TYPE_CLIENT
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
