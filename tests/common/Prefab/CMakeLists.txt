add_executable(APrefabTests APrefabTests.cpp)
target_link_libraries(APrefabTests PREFAB ECS COLLISION_RULES GTest::gtest GTest::gtest_main gcov)
add_test(NAME APrefabTests COMMAND APrefabTests)

add_executable(EntityPrefabManagerTests EntityPrefabManagerTests.cpp)
target_link_libraries(EntityPrefabManagerTests PREFAB ECS COLLISION_RULES GTest::gtest GTest::gtest_main gcov)
add_test(NAME EntityPrefabManagerTests COMMAND EntityPrefabManagerTests)
