set(SERVER_PACKET_HANDLERS_NAME ServerPacketHandlers)

add_library(${SERVER_PACKET_HANDLERS_NAME} STATIC
    DefaultPacketHandlers.cpp
    DefaultPacketHandlers.hpp
)

target_include_directories(${SERVER_PACKET_HANDLERS_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/common
)

target_link_libraries(${SERVER_PACKET_HANDLERS_NAME}
    PRIVATE Common::PacketHandlers
)

add_library(Server::PacketHandlers ALIAS ${SERVER_PACKET_HANDLERS_NAME})
