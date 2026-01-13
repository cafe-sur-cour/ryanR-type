add_executable(VelocityComponentTests component/VelocityComponentTests.cpp)
target_link_libraries(VelocityComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME VelocityComponentTests COMMAND VelocityComponentTests)

add_executable(TransformComponentTests component/TransformComponentTests.cpp)
target_link_libraries(TransformComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME TransformComponentTests COMMAND TransformComponentTests)

add_executable(PlayerTagTests component/PlayerTagTests.cpp)
target_link_libraries(PlayerTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME PlayerTagTests COMMAND PlayerTagTests)

add_executable(MobTagTests component/MobTagTests.cpp)
target_link_libraries(MobTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME MobTagTests COMMAND MobTagTests)

add_executable(PlayerProjectileTagTests component/PlayerProjectileTagTests.cpp)
target_link_libraries(PlayerProjectileTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME PlayerProjectileTagTests COMMAND PlayerProjectileTagTests)

add_executable(EnnemyProjectileTagTests component/EnnemyProjectileTagTests.cpp)
target_link_libraries(EnnemyProjectileTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME EnnemyProjectileTagTests COMMAND EnnemyProjectileTagTests)

add_executable(ObstacleTagTests component/ObstacleTagTests.cpp)
target_link_libraries(ObstacleTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ObstacleTagTests COMMAND ObstacleTagTests)

add_executable(ControllableTagTests component/ControllableTagTests.cpp)
target_link_libraries(ControllableTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ControllableTagTests COMMAND ControllableTagTests)

add_executable(ProjectilePassThroughTagTests component/ProjectilePassThroughTagTests.cpp)
target_link_libraries(ProjectilePassThroughTagTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ProjectilePassThroughTagTests COMMAND ProjectilePassThroughTagTests)

add_executable(MovementIntentComponentTests component/MovementIntentComponentTests.cpp)
target_link_libraries(MovementIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME MovementIntentComponentTests COMMAND MovementIntentComponentTests)

add_executable(SpeedComponentTests component/SpeedComponentTests.cpp)
target_link_libraries(SpeedComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME SpeedComponentTests COMMAND SpeedComponentTests)

add_executable(SpriteComponentTests component/SpriteComponentTests.cpp)
target_link_libraries(SpriteComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME SpriteComponentTests COMMAND SpriteComponentTests)

add_executable(AnimationComponentTests component/AnimationComponentTests.cpp)
target_link_libraries(AnimationComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME AnimationComponentTests COMMAND AnimationComponentTests)

add_executable(ScoreComponentTests component/ScoreComponentTests.cpp)
target_link_libraries(ScoreComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ScoreComponentTests COMMAND ScoreComponentTests)

add_executable(ScoreIntentComponentTests component/ScoreIntentComponentTests.cpp)
target_link_libraries(ScoreIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ScoreIntentComponentTests COMMAND ScoreIntentComponentTests)

add_executable(ColliderComponentTests component/ColliderComponentTests.cpp)
target_link_libraries(ColliderComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ColliderComponentTests COMMAND ColliderComponentTests)

add_executable(LifetimeComponentTests component/LifetimeComponentTests.cpp)
target_link_libraries(LifetimeComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME LifetimeComponentTests COMMAND LifetimeComponentTests)

add_executable(HealthComponentTests component/HealthComponentTests.cpp)
target_link_libraries(HealthComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME HealthComponentTests COMMAND HealthComponentTests)

add_executable(DamageComponentTests component/DamageComponentTests.cpp)
target_link_libraries(DamageComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME DamageComponentTests COMMAND DamageComponentTests)

add_executable(DamageIntentComponentTests component/DamageIntentComponentTests.cpp)
target_link_libraries(DamageIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME DamageIntentComponentTests COMMAND DamageIntentComponentTests)

add_executable(DeathIntentComponentTests component/DeathIntentComponentTests.cpp)
target_link_libraries(DeathIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME DeathIntentComponentTests COMMAND DeathIntentComponentTests)

add_executable(RegistryTests RegistryTests.cpp)
target_link_libraries(RegistryTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME RegistryTests COMMAND RegistryTests)

add_executable(MovementSystemTests system/MovementSystemTests.cpp)
target_link_libraries(MovementSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME MovementSystemTests COMMAND MovementSystemTests)

add_executable(LifetimeSystemTests system/LifetimeSystemTests.cpp)
target_link_libraries(LifetimeSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME LifetimeSystemTests COMMAND LifetimeSystemTests)

add_executable(HealthSystemTests system/HealthSystemTests.cpp)
target_link_libraries(HealthSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME HealthSystemTests COMMAND HealthSystemTests)

add_executable(DeathSystemTests system/DeathSystemTests.cpp)
target_link_libraries(DeathSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME DeathSystemTests COMMAND DeathSystemTests)

add_executable(OutOfBoundsSystemTests system/OutOfBoundsSystemTests.cpp)
target_link_libraries(OutOfBoundsSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME OutOfBoundsSystemTests COMMAND OutOfBoundsSystemTests)

add_executable(ScoreSystemTests system/ScoreSystemTests.cpp)
target_link_libraries(ScoreSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ScoreSystemTests COMMAND ScoreSystemTests)

add_executable(TriggerIntentComponentTests component/TriggerIntentComponentTests.cpp)
target_link_libraries(TriggerIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME TriggerIntentComponentTests COMMAND TriggerIntentComponentTests)

add_executable(InteractionConfigComponentTests component/InteractionConfigComponentTests.cpp)
target_link_libraries(InteractionConfigComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME InteractionConfigComponentTests COMMAND InteractionConfigComponentTests)

add_executable(TriggerSystemTests system/TriggerSystemTests.cpp)
target_link_libraries(TriggerSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME TriggerSystemTests COMMAND TriggerSystemTests)


add_executable(SpawnIntentComponentTests component/SpawnIntentComponentTests.cpp)
target_link_libraries(SpawnIntentComponentTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME SpawnIntentComponentTests COMMAND SpawnIntentComponentTests)

add_executable(SpawnSystemTests system/SpawnSystemTests.cpp)
target_link_libraries(SpawnSystemTests ECS PREFAB COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME SpawnSystemTests COMMAND SpawnSystemTests)
