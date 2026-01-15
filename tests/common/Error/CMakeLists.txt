add_executable(ErrorTest ErrorTests.cpp)
target_link_libraries(ErrorTest ECS PREFAB COLLISION_RULES PARSER GTest::gtest GTest::gtest_main gcov)
add_test(NAME ErrorTest COMMAND ErrorTest)
