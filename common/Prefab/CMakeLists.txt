set(PREFAB_LIB PREFAB)

add_subdirectory(entityPrefabManager)

set(PREFAB_SOURCES
    APrefab.cpp
    ParsedEntityPrefab.cpp
)

set(PREFAB_HEADERS
    APrefab.hpp
    IPrefab.hpp
    ParsedEntityPrefab.hpp
)

add_library(${PREFAB_LIB} STATIC ${PREFAB_SOURCES} ${PREFAB_HEADERS})

target_include_directories(${PREFAB_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PREFAB_LIB} PUBLIC
    ECS
    ENTITY_PREFAB_MANAGER
    nlohmann_json::nlohmann_json
)
