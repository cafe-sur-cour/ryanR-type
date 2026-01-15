add_executable(JsonLoaderTests JsonLoaderTest.cpp)
target_link_libraries(JsonLoaderTests PARSER_UTILS ERROR GTest::gtest GTest::gtest_main gcov nlohmann_json::nlohmann_json)
add_test(NAME JsonLoaderTests COMMAND JsonLoaderTests)

add_executable(JsonValidationTests JsonValidationTest.cpp)
target_link_libraries(JsonValidationTests PARSER_UTILS GTest::gtest GTest::gtest_main gcov nlohmann_json::nlohmann_json)
add_test(NAME JsonValidationTests COMMAND JsonValidationTests)
