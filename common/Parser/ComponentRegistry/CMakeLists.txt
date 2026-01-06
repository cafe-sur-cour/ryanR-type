cmake_minimum_required(VERSION 3.17)

project(ComponentRegistry)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    ComponentRegistry.cpp
)

set(HEADERS
    ComponentMetadata.hpp
    ComponentRegistry.hpp
    ComponentRegistrar.hpp
)

add_library(ComponentRegistry STATIC ${SOURCES} ${HEADERS})

target_include_directories(ComponentRegistry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(ComponentRegistry PUBLIC nlohmann_json::nlohmann_json)
