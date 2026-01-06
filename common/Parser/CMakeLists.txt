set(PARSER_LIB PARSER)

add_subdirectory(Utils)
add_subdirectory(ComponentRegistry)
add_subdirectory(ComposantParser)
add_subdirectory(EntityParser)
add_subdirectory(MapParser)
add_subdirectory(Animation)

set(PARSER_SOURCES
    Parser.cpp
    CollisionRulesParser.cpp
)

set(PARSER_HEADERS
    Parser.hpp
    ParserParam.hpp
    CollisionRulesParser.hpp
)

add_library(${PARSER_LIB} STATIC ${PARSER_SOURCES} ${PARSER_HEADERS})

target_include_directories(${PARSER_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PARSER_LIB} PUBLIC
    PREFAB
    ComponentRegistry
    COMPOSANT_PARSER
    ENTITY_PARSER
    MAP_PARSER
    ANIMATION
    ERROR
    COLLISION_RULES
    nlohmann_json::nlohmann_json
    parser_utils
)
