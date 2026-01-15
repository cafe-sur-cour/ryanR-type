add_subdirectory(component)
add_subdirectory(system)

add_executable(RegistryTests RegistryTests.cpp)
target_link_libraries(RegistryTests ECS PREFAB COLLISION_RULES GTest::gtest GTest::gtest_main)
if(BUILD_COVERAGE)
    target_link_libraries(RegistryTests gcov)
endif()
add_test(NAME RegistryTests COMMAND RegistryTests)
