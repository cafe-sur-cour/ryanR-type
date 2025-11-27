set(SERVER_BIN_NAME r-type_server)

add_executable(${SERVER_BIN_NAME}
    main.cpp
    IServer.hpp
    ServerConfig.hpp
    ServerConfig.cpp
    Server.hpp
    Server.cpp
    Utils.cpp
    Utils.hpp
    Core.cpp
    Core.hpp
)

add_subdirectory(gsm)
add_subdirectory(initRessourcesManager)

find_package(asio CONFIG REQUIRED)

target_include_directories(${SERVER_BIN_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${SERVER_BIN_NAME}
    PRIVATE
        ECS
        ERROR
        SIGNAL
        DLLoader
        Network
        Packet
        asio::asio
)

add_custom_command(TARGET ${SERVER_BIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SERVER_BIN_NAME}> ${CMAKE_SOURCE_DIR}/
    COMMENT "Copying ${SERVER_BIN_NAME} to root directory"
)
