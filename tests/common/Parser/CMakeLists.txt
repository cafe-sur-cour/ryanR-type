add_executable(ParserTests ParserTests.cpp)
target_link_libraries(ParserTests PARSER PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME ParserTests COMMAND ParserTests)

add_executable(MapParserTests MapParserTests.cpp)
target_link_libraries(MapParserTests MAP_PARSER PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main)
add_test(NAME MapParserTests COMMAND MapParserTests)

add_executable(JsonLoaderTests Utils/JsonLoaderTest.cpp)
target_link_libraries(JsonLoaderTests PARSER_UTILS ERROR GTest::gtest GTest::gtest_main nlohmann_json::nlohmann_json)
add_test(NAME JsonLoaderTests COMMAND JsonLoaderTests)

add_executable(JsonValidationTests Utils/JsonValidationTest.cpp)
target_link_libraries(JsonValidationTests PARSER_UTILS GTest::gtest GTest::gtest_main nlohmann_json::nlohmann_json)
add_test(NAME JsonValidationTests COMMAND JsonValidationTests)
