add_subdirectory(serializer)

add_executable(PacketTests packettest.cpp)
target_link_libraries(PacketTests Packet Buffer CommonPacketHandlers PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main gcov)
add_test(NAME PacketTests COMMAND PacketTests)

add_executable(AdditionalPacketTests AdditionalPacketTests.cpp)
target_link_libraries(AdditionalPacketTests Packet Buffer CommonPacketHandlers PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main gcov)
add_test(NAME AdditionalPacketTests COMMAND AdditionalPacketTests)

add_executable(EcsConversionTests EcsConversionTests.cpp)
target_link_libraries(EcsConversionTests r-type_server_lib ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main gcov)
add_test(NAME EcsConversionTests COMMAND EcsConversionTests)
