set(PACKET_LIB_NAME Packet)

add_library(${PACKET_LIB_NAME} SHARED
    IPacket.hpp
    Packet.hpp
    Packet.cpp
    PacketTypes.cpp
)

target_include_directories(${PACKET_LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PACKET_LIB_NAME}
    PRIVATE ERROR
)

add_subdirectory(serializer)


add_custom_command(TARGET ${PACKET_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PACKET_LIB_NAME}> ${CMAKE_SOURCE_DIR}/librairies/
)
