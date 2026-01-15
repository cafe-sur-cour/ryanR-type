add_subdirectory(Utils)

add_executable(ParserTests ParserTests.cpp)
target_link_libraries(ParserTests PARSER PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main gcov)
add_test(NAME ParserTests COMMAND ParserTests)

add_executable(MapParserTests MapParserTests.cpp)
target_link_libraries(MapParserTests MAP_PARSER PREFAB ECS COLLISION_RULES ERROR GTest::gtest GTest::gtest_main gcov)
add_test(NAME MapParserTests COMMAND MapParserTests)
