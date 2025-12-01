set(COMPOSANT_PARSER_LIB COMPOSANT_PARSER)

set(COMPOSANT_PARSER_SOURCES
    ComposantParser.cpp
)

set(COMPOSANT_PARSER_HEADERS
    ComposantParser.hpp
)

add_library(${COMPOSANT_PARSER_LIB} STATIC ${COMPOSANT_PARSER_SOURCES} ${COMPOSANT_PARSER_HEADERS})

target_include_directories(${COMPOSANT_PARSER_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(${COMPOSANT_PARSER_LIB} PUBLIC
    ECS
    nlohmann_json::nlohmann_json
)