set(ENTITY_PARSER_LIB ENTITY_PARSER)

set(ENTITY_PARSER_SOURCES
    EntityParser.cpp
)

set(ENTITY_PARSER_HEADERS
    EntityParser.hpp
)

add_library(${ENTITY_PARSER_LIB} STATIC ${ENTITY_PARSER_SOURCES} ${ENTITY_PARSER_HEADERS})

target_include_directories(${ENTITY_PARSER_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(${ENTITY_PARSER_LIB} PUBLIC
    COMPOSANT_PARSER
    PREFAB
    nlohmann_json::nlohmann_json
)