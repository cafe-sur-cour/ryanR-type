set(PACKET_LIB_NAME Packet)

add_library(${PACKET_LIB_NAME} SHARED
    IPacket.hpp
    Packet.hpp
    Packet.cpp
    PacketTypes.cpp
)

target_include_directories(${PACKET_LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(serializer)
