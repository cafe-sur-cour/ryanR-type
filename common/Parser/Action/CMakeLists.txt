set(ACTION_LIB ACTION)

set(ACTION_SOURCES
    ActionFactory.cpp
    ActionRegistry.cpp
)

set(ACTION_HEADERS
    ActionFactory.hpp
)

add_library(${ACTION_LIB} STATIC ${ACTION_SOURCES} ${ACTION_HEADERS})

target_include_directories(${ACTION_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(${ACTION_LIB} PUBLIC
    ECS
)
