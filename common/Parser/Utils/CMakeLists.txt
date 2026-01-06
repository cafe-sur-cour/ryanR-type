add_library(parser_utils STATIC
    JsonLoader.cpp
    JsonValidation.cpp
)

target_include_directories(parser_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(parser_utils
    nlohmann_json::nlohmann_json
    ERROR
)
