add_executable(parser_utils_tests
    JsonLoaderTest.cpp
    JsonValidationTest.cpp
)

target_link_libraries(parser_utils_tests
    parser_utils
    ERROR
    GTest::gtest
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)

add_test(NAME ParserUtilsTests COMMAND parser_utils_tests)
