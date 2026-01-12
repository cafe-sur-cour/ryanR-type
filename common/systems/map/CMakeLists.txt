cmake_minimum_required(VERSION 3.16)

set(SOURCES
    MapGeneratorSystem.cpp
)

add_library(MapGeneratorSystem ${SOURCES})

target_include_directories(MapGeneratorSystem PUBLIC
    ${PROJECT_SOURCE_DIR}
)
