set(COMMON_PACKET_HANDLERS_NAME CommonPacketHandlers)

add_library(${COMMON_PACKET_HANDLERS_NAME} STATIC
    DefaultPacketHandlers.cpp
    DefaultPacketHandlers.hpp
)

target_include_directories(${COMMON_PACKET_HANDLERS_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/libs/Packet
)

target_link_libraries(${COMMON_PACKET_HANDLERS_NAME}
    PUBLIC Packet
)

add_library(Common::PacketHandlers ALIAS ${COMMON_PACKET_HANDLERS_NAME})
