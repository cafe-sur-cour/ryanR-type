set(SERVER_LIB_NAME r-type_server_lib)

add_library(${SERVER_LIB_NAME}
    IServer.hpp
    ServerConfig.hpp
    ServerConfig.cpp
    Server.hpp
    Server.cpp
    Utils.cpp
    Utils.hpp
    Core.cpp
    Core.hpp
    ServerLibsLoading.cpp
)

find_package(asio CONFIG REQUIRED)

target_link_libraries(${SERVER_LIB_NAME}
    PUBLIC
        ECS
        ERROR
        SIGNAL
        DLLoader
        NetworkServer
        NetworkClient
        DEBUG
        Packet
        asio::asio
        nlohmann_json::nlohmann_json
)

add_subdirectory(gsm)
add_subdirectory(initResourcesManager)

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
