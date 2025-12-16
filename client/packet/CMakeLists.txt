set(CLIENT_PACKET_HANDLERS_NAME ClientPacketHandlers)

add_library(${CLIENT_PACKET_HANDLERS_NAME} STATIC
	DefaultPacketHandlers.cpp
	DefaultPacketHandlers.hpp
)

target_include_directories(${CLIENT_PACKET_HANDLERS_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_SOURCE_DIR}/common
)

target_link_libraries(${CLIENT_PACKET_HANDLERS_NAME}
	PRIVATE Common::PacketHandlers
)

add_library(Client::PacketHandlers ALIAS ${CLIENT_PACKET_HANDLERS_NAME})
