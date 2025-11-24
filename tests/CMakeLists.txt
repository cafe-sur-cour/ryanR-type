find_package(GTest REQUIRED)

add_executable(ServerConfigTests server/ServerConfigTests.cpp)
target_link_libraries(ServerConfigTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME ServerConfigTests COMMAND ServerConfigTests)

add_executable(ECSTests common/ECSTests.cpp)
target_link_libraries(ECSTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME ECSTests COMMAND ECSTests)

add_executable(BufferTests server/buffer/circularBufferTest.cpp)
target_link_libraries(BufferTests Buffer GTest::gtest GTest::gtest_main)
add_test(NAME BufferTests COMMAND BufferTests)

add_executable(PacketTests server/packet/packettest.cpp)
target_link_libraries(PacketTests Packet Buffer GTest::gtest GTest::gtest_main)
add_test(NAME PacketTests COMMAND PacketTests)