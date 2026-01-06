set(SOURCES
    ComponentRegistry.cpp
)

set(HEADERS
    ComponentMetadata.hpp
    ComponentRegistry.hpp
    ComponentRegistrar.hpp
)

add_library(ComponentRegistry STATIC ${SOURCES} ${HEADERS})

set_target_properties(ComponentRegistry PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(ComponentRegistry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(ComponentRegistry PUBLIC nlohmann_json::nlohmann_json)
