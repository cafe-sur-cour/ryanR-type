set(SERVER_CORE_LIB ServerCore)

add_library(${SERVER_CORE_LIB} STATIC
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

find_package(asio CONFIG REQUIRED)

target_link_libraries(${SERVER_CORE_LIB}
    PUBLIC
        ECS
        ERROR
        SIGNAL
        DLLoader
        Network
        Packet
        asio::asio
)

target_include_directories(${SERVER_CORE_LIB}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(gsm)
add_subdirectory(initRessourcesManager)

set(SERVER_BIN_NAME r-type_server)

add_executable(${SERVER_BIN_NAME}
    main.cpp
)

target_link_libraries(${SERVER_BIN_NAME}
    PRIVATE
        ${SERVER_CORE_LIB}
        ServerGSM
        ServerInit
)

add_custom_command(TARGET ${SERVER_BIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SERVER_BIN_NAME}> ${CMAKE_SOURCE_DIR}/
    COMMENT "Copying ${SERVER_BIN_NAME} to root directory"
)
