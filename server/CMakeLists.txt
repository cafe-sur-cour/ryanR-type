set(SERVER_BIN_NAME r-type_server)

add_executable(${SERVER_BIN_NAME}
    main.cpp
    IServer.hpp
    ServerConfig.hpp
    ServerConfig.cpp
    Server.hpp
    Server.cpp
)

find_package(asio CONFIG REQUIRED)

target_link_libraries(${SERVER_BIN_NAME}
    PUBLIC
        ECS
        DLLoader
        asio::asio
)

add_subdirectory(gsm)
add_subdirectory(initRessourcesManager)

add_custom_command(TARGET ${SERVER_BIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SERVER_BIN_NAME}> ${CMAKE_SOURCE_DIR}/
    COMMENT "Copying ${SERVER_BIN_NAME} to root directory"
)
