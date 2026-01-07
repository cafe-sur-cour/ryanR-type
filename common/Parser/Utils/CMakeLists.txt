add_library(PARSER_UTILS STATIC
    JsonLoader.cpp
    JsonValidation.cpp
)

target_include_directories(PARSER_UTILS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(PARSER_UTILS
    nlohmann_json::nlohmann_json
    ERROR
)
