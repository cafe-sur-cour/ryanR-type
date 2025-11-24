add_executable(ServerConfigTests server/ServerConfigTests.cpp)
target_link_libraries(ServerConfigTests server GTest::gtest GTest::gtest_main)
add_test(NAME ServerConfigTests COMMAND ServerConfigTests)