add_executable(SerializerTests SerializerTests.cpp)
target_link_libraries(SerializerTests Packet GTest::gtest GTest::gtest_main gcov)
add_test(NAME SerializerTests COMMAND SerializerTests)
