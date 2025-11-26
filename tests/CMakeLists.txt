find_package(GTest REQUIRED)

add_executable(ServerConfigTests server/ServerConfigTests.cpp)
target_link_libraries(ServerConfigTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME ServerConfigTests COMMAND ServerConfigTests)

add_executable(ECSTests common/ECSTests.cpp)
target_link_libraries(ECSTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME ECSTests COMMAND ECSTests)

add_executable(Vector2fTests common/types/Vector2fTests.cpp)
target_link_libraries(Vector2fTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME Vector2fTests COMMAND Vector2fTests)

add_executable(VelocityComponentTests common/ECS/component/VelocityComponentTests.cpp)
target_link_libraries(VelocityComponentTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME VelocityComponentTests COMMAND VelocityComponentTests)

add_executable(TransformComponentTests common/ECS/component/TransformComponentTests.cpp)
target_link_libraries(TransformComponentTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME TransformComponentTests COMMAND TransformComponentTests)

add_executable(PlayerTagTests common/ECS/component/PlayerTagTests.cpp)
target_link_libraries(PlayerTagTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME PlayerTagTests COMMAND PlayerTagTests)

add_executable(MobTagTests common/ECS/component/MobTagTests.cpp)
target_link_libraries(MobTagTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME MobTagTests COMMAND MobTagTests)

add_executable(ProjectileTagTests common/ECS/component/ProjectileTagTests.cpp)
target_link_libraries(ProjectileTagTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME ProjectileTagTests COMMAND ProjectileTagTests)

add_executable(ObstacleTagTests common/ECS/component/ObstacleTagTests.cpp)
target_link_libraries(ObstacleTagTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME ObstacleTagTests COMMAND ObstacleTagTests)

add_executable(MovementIntentComponentTests common/ECS/component/MovementIntentComponentTests.cpp)
target_link_libraries(MovementIntentComponentTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME MovementIntentComponentTests COMMAND MovementIntentComponentTests)

add_executable(SpeedComponentTests common/ECS/component/SpeedComponentTests.cpp)
target_link_libraries(SpeedComponentTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME SpeedComponentTests COMMAND SpeedComponentTests)

add_executable(RegistryTests common/ECS/RegistryTests.cpp)
target_link_libraries(RegistryTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME RegistryTests COMMAND RegistryTests)

add_executable(MovementInputSystemTests common/ECS/MovementInputSystemTests.cpp)
target_link_libraries(MovementInputSystemTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME MovementInputSystemTests COMMAND MovementInputSystemTests)

add_executable(MovementSystemTests common/ECS/system/MovementSystemTests.cpp)
target_link_libraries(MovementSystemTests ECS GTest::gtest GTest::gtest_main)
add_test(NAME MovementSystemTests COMMAND MovementSystemTests)

add_executable(BufferTests server/buffer/circularBufferTest.cpp)
target_link_libraries(BufferTests Buffer GTest::gtest GTest::gtest_main)
add_test(NAME BufferTests COMMAND BufferTests)

add_executable(PacketTests server/packet/packettest.cpp)
target_link_libraries(PacketTests Packet Buffer GTest::gtest GTest::gtest_main)
add_test(NAME PacketTests COMMAND PacketTests)

add_executable(UtilsFuncTests server/utils/utils-func-test.cpp)
target_link_libraries(UtilsFuncTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME UtilsFuncTests COMMAND UtilsFuncTests)
add_executable(SerializerTests server/packet/serializer/SerializerTests.cpp)
target_link_libraries(SerializerTests Packet GTest::gtest GTest::gtest_main)
add_test(NAME SerializerTests COMMAND SerializerTests)

add_executable(ErrorTests common/Error/Error-test.cpp)
target_link_libraries(ErrorTests ERROR GTest::gtest GTest::gtest_main)
add_test(NAME ErrorTests COMMAND ErrorTests)
