set(MAP_PARSER_LIB MAP_PARSER)

set(MAP_PARSER_SOURCES
    MapParser.cpp
    MapHandler.cpp
)

set(MAP_PARSER_HEADERS
    MapParser.hpp
    MapHandler.hpp
)

add_library(${MAP_PARSER_LIB} STATIC ${MAP_PARSER_SOURCES} ${MAP_PARSER_HEADERS})

target_include_directories(${MAP_PARSER_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(${MAP_PARSER_LIB} PUBLIC
    ECS
    ENTITY_PREFAB_MANAGER
    ERROR
    nlohmann_json::nlohmann_json
)