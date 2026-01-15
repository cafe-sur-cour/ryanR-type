add_executable(BufferTests CircularBufferTest.cpp)
target_link_libraries(BufferTests Buffer GTest::gtest GTest::gtest_main gcov)
add_test(NAME BufferTests COMMAND BufferTests)
