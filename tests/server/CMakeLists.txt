find_package(GTest REQUIRED)

add_executable(ServerConfigTests ServerConfigTests.cpp)
target_link_libraries(ServerConfigTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME ServerConfigTests COMMAND ServerConfigTests)

add_executable(BufferTests buffer/circularBufferTest.cpp)
target_link_libraries(BufferTests Buffer GTest::gtest GTest::gtest_main)
add_test(NAME BufferTests COMMAND BufferTests)

add_executable(PacketTests packet/packettest.cpp)
target_link_libraries(PacketTests Packet Buffer CommonPacketHandlers PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME PacketTests COMMAND PacketTests)

add_executable(AdditionalPacketTests packet/AdditionalPacketTests.cpp)
target_link_libraries(AdditionalPacketTests Packet Buffer CommonPacketHandlers PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME AdditionalPacketTests COMMAND AdditionalPacketTests)

add_executable(EcsConversionTests packet/EcsConversionTests.cpp)
target_link_libraries(EcsConversionTests r-type_server_lib ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME EcsConversionTests COMMAND EcsConversionTests)

add_executable(AdditionalServerConfigTests AdditionalServerConfigTests.cpp)
target_link_libraries(AdditionalServerConfigTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME AdditionalServerConfigTests COMMAND AdditionalServerConfigTests)

add_executable(UtilsFuncTests utils/utils-func-test.cpp)
target_link_libraries(UtilsFuncTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME UtilsFuncTests COMMAND UtilsFuncTests)

add_executable(SerializerTests packet/serializer/SerializerTests.cpp)
target_link_libraries(SerializerTests Packet GTest::gtest GTest::gtest_main)
add_test(NAME SerializerTests COMMAND SerializerTests)
