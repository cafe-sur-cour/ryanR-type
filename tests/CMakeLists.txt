find_package(GTest REQUIRED)

add_executable(ServerConfigTests server/ServerConfigTests.cpp)
target_link_libraries(ServerConfigTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME ServerConfigTests COMMAND ServerConfigTests)

add_executable(ECSTests common/ECSTests.cpp)
target_link_libraries(ECSTests ECS PREFAB COLLISION_RULES PARSER GTest::gtest GTest::gtest_main)
add_test(NAME ECSTests COMMAND ECSTests)

add_executable(Vector2fTests common/types/Vector2fTests.cpp)
target_link_libraries(Vector2fTests ECS PREFAB COLLISION_RULES PARSER GTest::gtest GTest::gtest_main)
add_test(NAME Vector2fTests COMMAND Vector2fTests)

add_executable(FRectTests common/types/FRectTests.cpp)
target_link_libraries(FRectTests ECS PREFAB COLLISION_RULES PARSER GTest::gtest GTest::gtest_main)
add_test(NAME FRectTests COMMAND FRectTests)

add_executable(ChronoTests common/types/ChronoTests.cpp)
target_link_libraries(ChronoTests ECS PREFAB COLLISION_RULES PARSER GTest::gtest GTest::gtest_main)
add_test(NAME ChronoTests COMMAND ChronoTests)

add_executable(SpatialGridTests common/SpatialGridTests.cpp)
target_link_libraries(SpatialGridTests ECS PREFAB COLLISION_RULES PARSER GTest::gtest GTest::gtest_main)
add_test(NAME SpatialGridTests COMMAND SpatialGridTests)

add_executable(VelocityComponentTests common/ECS/component/VelocityComponentTests.cpp)
target_link_libraries(VelocityComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME VelocityComponentTests COMMAND VelocityComponentTests)

add_executable(TransformComponentTests common/ECS/component/TransformComponentTests.cpp)
target_link_libraries(TransformComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME TransformComponentTests COMMAND TransformComponentTests)

add_executable(PlayerTagTests common/ECS/component/PlayerTagTests.cpp)
target_link_libraries(PlayerTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME PlayerTagTests COMMAND PlayerTagTests)

add_executable(MobTagTests common/ECS/component/MobTagTests.cpp)
target_link_libraries(MobTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME MobTagTests COMMAND MobTagTests)

add_executable(PlayerProjectileTagTests common/ECS/component/PlayerProjectileTagTests.cpp)
target_link_libraries(PlayerProjectileTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME PlayerProjectileTagTests COMMAND PlayerProjectileTagTests)

add_executable(EnnemyProjectileTagTests common/ECS/component/EnnemyProjectileTagTests.cpp)
target_link_libraries(EnnemyProjectileTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME EnnemyProjectileTagTests COMMAND EnnemyProjectileTagTests)

add_executable(ObstacleTagTests common/ECS/component/ObstacleTagTests.cpp)
target_link_libraries(ObstacleTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ObstacleTagTests COMMAND ObstacleTagTests)

add_executable(ControllableTagTests common/ECS/component/ControllableTagTests.cpp)
target_link_libraries(ControllableTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ControllableTagTests COMMAND ControllableTagTests)

add_executable(ProjectilePassThroughTagTests common/ECS/component/ProjectilePassThroughTagTests.cpp)
target_link_libraries(ProjectilePassThroughTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ProjectilePassThroughTagTests COMMAND ProjectilePassThroughTagTests)

add_executable(MovementIntentComponentTests common/ECS/component/MovementIntentComponentTests.cpp)
target_link_libraries(MovementIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME MovementIntentComponentTests COMMAND MovementIntentComponentTests)

add_executable(SpeedComponentTests common/ECS/component/SpeedComponentTests.cpp)
target_link_libraries(SpeedComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME SpeedComponentTests COMMAND SpeedComponentTests)

add_executable(SpriteComponentTests common/ECS/component/SpriteComponentTests.cpp)
target_link_libraries(SpriteComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME SpriteComponentTests COMMAND SpriteComponentTests)

add_executable(AnimationComponentTests common/ECS/component/AnimationComponentTests.cpp)
target_link_libraries(AnimationComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME AnimationComponentTests COMMAND AnimationComponentTests)

add_executable(ScoreComponentTests common/ECS/component/ScoreComponentTests.cpp)
target_link_libraries(ScoreComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ScoreComponentTests COMMAND ScoreComponentTests)

add_executable(ScoreIntentComponentTests common/ECS/component/ScoreIntentComponentTests.cpp)
target_link_libraries(ScoreIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ScoreIntentComponentTests COMMAND ScoreIntentComponentTests)

add_executable(APrefabTests common/Prefab/APrefabTests.cpp)
target_link_libraries(APrefabTests PREFAB ECS COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME APrefabTests COMMAND APrefabTests)

add_executable(EntityPrefabManagerTests common/Prefab/EntityPrefabManagerTests.cpp)
target_link_libraries(EntityPrefabManagerTests PREFAB ECS COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME EntityPrefabManagerTests COMMAND EntityPrefabManagerTests)

add_executable(ColliderComponentTests common/ECS/component/ColliderComponentTests.cpp)
target_link_libraries(ColliderComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ColliderComponentTests COMMAND ColliderComponentTests)

add_executable(LifetimeComponentTests common/ECS/component/LifetimeComponentTests.cpp)
target_link_libraries(LifetimeComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME LifetimeComponentTests COMMAND LifetimeComponentTests)

add_executable(HealthComponentTests common/ECS/component/HealthComponentTests.cpp)
target_link_libraries(HealthComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME HealthComponentTests COMMAND HealthComponentTests)

add_executable(DamageComponentTests common/ECS/component/DamageComponentTests.cpp)
target_link_libraries(DamageComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME DamageComponentTests COMMAND DamageComponentTests)

add_executable(DamageIntentComponentTests common/ECS/component/DamageIntentComponentTests.cpp)
target_link_libraries(DamageIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME DamageIntentComponentTests COMMAND DamageIntentComponentTests)

add_executable(DeathIntentComponentTests common/ECS/component/DeathIntentComponentTests.cpp)
target_link_libraries(DeathIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME DeathIntentComponentTests COMMAND DeathIntentComponentTests)

add_executable(RegistryTests common/ECS/RegistryTests.cpp)
target_link_libraries(RegistryTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME RegistryTests COMMAND RegistryTests)

add_executable(MovementSystemTests common/ECS/system/MovementSystemTests.cpp)
target_link_libraries(MovementSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME MovementSystemTests COMMAND MovementSystemTests)

add_executable(LifetimeSystemTests common/ECS/system/LifetimeSystemTests.cpp)
target_link_libraries(LifetimeSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME LifetimeSystemTests COMMAND LifetimeSystemTests)

add_executable(HealthSystemTests common/ECS/system/HealthSystemTests.cpp)
target_link_libraries(HealthSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME HealthSystemTests COMMAND HealthSystemTests)

add_executable(DeathSystemTests common/ECS/system/DeathSystemTests.cpp)
target_link_libraries(DeathSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME DeathSystemTests COMMAND DeathSystemTests)

add_executable(OutOfBoundsSystemTests common/ECS/system/OutOfBoundsSystemTests.cpp)
target_link_libraries(OutOfBoundsSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME OutOfBoundsSystemTests COMMAND OutOfBoundsSystemTests)

add_executable(ScoreSystemTests common/ECS/system/ScoreSystemTests.cpp)
target_link_libraries(ScoreSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ScoreSystemTests COMMAND ScoreSystemTests)

add_executable(BufferTests server/buffer/circularBufferTest.cpp)
target_link_libraries(BufferTests Buffer GTest::gtest GTest::gtest_main)
add_test(NAME BufferTests COMMAND BufferTests)

add_executable(PacketTests server/packet/packettest.cpp)
target_link_libraries(PacketTests Packet Buffer CommonPacketHandlers PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME PacketTests COMMAND PacketTests)

add_executable(AdditionalPacketTests server/packet/AdditionalPacketTests.cpp)
target_link_libraries(AdditionalPacketTests Packet Buffer CommonPacketHandlers PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME AdditionalPacketTests COMMAND AdditionalPacketTests)

add_executable(EcsConversionTests server/packet/EcsConversionTests.cpp)
target_link_libraries(EcsConversionTests r-type_server_lib ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME EcsConversionTests COMMAND EcsConversionTests)

add_executable(AdditionalServerConfigTests server/AdditionalServerConfigTests.cpp)
target_link_libraries(AdditionalServerConfigTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME AdditionalServerConfigTests COMMAND AdditionalServerConfigTests)

add_executable(UtilsFuncTests server/utils/utils-func-test.cpp)
target_link_libraries(UtilsFuncTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME UtilsFuncTests COMMAND UtilsFuncTests)
add_executable(SerializerTests server/packet/serializer/SerializerTests.cpp)
target_link_libraries(SerializerTests Packet GTest::gtest GTest::gtest_main)
add_test(NAME SerializerTests COMMAND SerializerTests)

add_executable(ClientGameStateConversionsTests 
    client/ClientGameStateConversionsTests.cpp
    client/ClientNetworkStub.cpp
    ../client/ClientGameStateConversions.cpp
    ../common/debug.cpp
)
target_link_libraries(ClientGameStateConversionsTests ECS PREFAB COLLISION_RULES PARSER DEBUG GTest::gtest GTest::gtest_main)
target_include_directories(ClientGameStateConversionsTests PRIVATE ${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/client ${CMAKE_BINARY_DIR})
target_compile_definitions(ClientGameStateConversionsTests PUBLIC R_TYPE_CLIENT)
add_test(NAME ClientGameStateConversionsTests COMMAND ClientGameStateConversionsTests)


add_executable(TriggerIntentComponentTests common/ECS/component/TriggerIntentComponentTests.cpp)
target_link_libraries(TriggerIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME TriggerIntentComponentTests COMMAND TriggerIntentComponentTests)

add_executable(InteractionConfigComponentTests common/ECS/component/InteractionConfigComponentTests.cpp)
target_link_libraries(InteractionConfigComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME InteractionConfigComponentTests COMMAND InteractionConfigComponentTests)

add_executable(TriggerSystemTests common/ECS/system/TriggerSystemTests.cpp)
target_link_libraries(TriggerSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME TriggerSystemTests COMMAND TriggerSystemTests)

add_executable(ParserTests common/Parser/ParserTests.cpp)
target_link_libraries(ParserTests PARSER PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME ParserTests COMMAND ParserTests)

add_executable(MapParserTests common/Parser/MapParserTests.cpp)
target_link_libraries(MapParserTests MAP_PARSER PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME MapParserTests COMMAND MapParserTests)

add_executable(SpawnIntentComponentTests common/ECS/component/SpawnIntentComponentTests.cpp)
target_link_libraries(SpawnIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME SpawnIntentComponentTests COMMAND SpawnIntentComponentTests)

add_executable(SpawnSystemTests common/ECS/system/SpawnSystemTests.cpp)
target_link_libraries(SpawnSystemTests ECS PREFAB COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME SpawnSystemTests COMMAND SpawnSystemTests)

add_executable(JsonLoaderTests
    common/Parser/Utils/JsonLoaderTest.cpp
)
target_link_libraries(JsonLoaderTests PARSER_UTILS ERROR GTest::gtest GTest::gtest_main nlohmann_json::nlohmann_json)
add_test(NAME JsonLoaderTests COMMAND JsonLoaderTests)

add_executable(JsonValidationTests
    common/Parser/Utils/JsonValidationTest.cpp
)
target_link_libraries(JsonValidationTests PARSER_UTILS GTest::gtest GTest::gtest_main nlohmann_json::nlohmann_json)
add_test(NAME JsonValidationTests COMMAND JsonValidationTests)

add_subdirectory(server)
add_subdirectory(common)
