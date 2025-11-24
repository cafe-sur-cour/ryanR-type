set(SERVER_BIN_NAME r-type_server)

add_executable(${SERVER_BIN_NAME}
    main.cpp
    IServer.hpp
    ServerConfig.hpp
    ServerConfig.cpp
    Server.hpp
    Server.cpp
)

target_link_libraries(${SERVER_BIN_NAME}
    PUBLIC
        ECS
        DLLoader
)

add_subdirectory(gsm)
