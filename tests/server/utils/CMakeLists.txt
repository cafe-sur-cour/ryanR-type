add_executable(UtilsFuncTests UtilsFuncTest.cpp)
target_link_libraries(UtilsFuncTests r-type_server_lib GTest::gtest GTest::gtest_main)
add_test(NAME UtilsFuncTests COMMAND UtilsFuncTests)
