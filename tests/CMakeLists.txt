find_package(GTest REQUIRED)

add_executable(ServerConfigTests server/ServerConfigTests.cpp)
target_link_libraries(ServerConfigTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME ServerConfigTests COMMAND ServerConfigTests)