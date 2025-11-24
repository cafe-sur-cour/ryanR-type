target_sources(${NETWORK_LIB_NAME} PUBLIC
    IPacket.hpp
    Packet.hpp
    Packet.cpp
)

add_subdirectory(serializer)
