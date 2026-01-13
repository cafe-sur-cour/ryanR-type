add_executable(MovementSystemTests MovementSystemTests.cpp)
target_link_libraries(MovementSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME MovementSystemTests COMMAND MovementSystemTests)

add_executable(LifetimeSystemTests LifetimeSystemTests.cpp)
target_link_libraries(LifetimeSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME LifetimeSystemTests COMMAND LifetimeSystemTests)

add_executable(HealthSystemTests HealthSystemTests.cpp)
target_link_libraries(HealthSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME HealthSystemTests COMMAND HealthSystemTests)

add_executable(DeathSystemTests DeathSystemTests.cpp)
target_link_libraries(DeathSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME DeathSystemTests COMMAND DeathSystemTests)

add_executable(OutOfBoundsSystemTests OutOfBoundsSystemTests.cpp)
target_link_libraries(OutOfBoundsSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME OutOfBoundsSystemTests COMMAND OutOfBoundsSystemTests)

add_executable(ScoreSystemTests ScoreSystemTests.cpp)
target_link_libraries(ScoreSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME ScoreSystemTests COMMAND ScoreSystemTests)

add_executable(TriggerSystemTests TriggerSystemTests.cpp)
target_link_libraries(TriggerSystemTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
add_test(NAME TriggerSystemTests COMMAND TriggerSystemTests)

add_executable(SpawnSystemTests SpawnSystemTests.cpp)
target_link_libraries(SpawnSystemTests ECS PREFAB COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME SpawnSystemTests COMMAND SpawnSystemTests)
