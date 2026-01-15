add_subdirectory(buffer)
add_subdirectory(packet)
add_subdirectory(utils)

add_executable(AdditionalServerConfigTests AdditionalServerConfigTests.cpp)
target_link_libraries(AdditionalServerConfigTests r-type_server_lib GTest::gtest GTest::gtest_main gcov)
add_test(NAME AdditionalServerConfigTests COMMAND AdditionalServerConfigTests)

add_executable(ServerConfigTests ServerConfigTests.cpp)
target_link_libraries(ServerConfigTests r-type_server_lib GTest::gtest GTest::gtest_main gcov)
add_test(NAME ServerConfigTests COMMAND ServerConfigTests)
