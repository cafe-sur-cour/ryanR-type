cmake_minimum_required(VERSION 3.23)

project(r-type)

if(WIN32)
    set(CMAKE_C_COMPILER "CL.exe")
    set(CMAKE_CXX_COMPILER "CL.exe")
elseif(UNIX)
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DASIO_STANDALONE)
endif()

find_package(asio CONFIG REQUIRED)
option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_CLIENT "Build client and multimedia library" ON)
option(BUILD_SERVER "Build server" ON)

if(APPLE)
    add_compile_options(-Wall -Wextra -Werror -Wshadow -Wpedantic -Wconversion -Wsign-conversion -Wfloat-equal -Wcast-align -Wformat=2 -Wnull-dereference -Wdouble-promotion)
elseif(UNIX)
    add_compile_options(-Wall -Wextra -Werror -Wshadow -Wpedantic -Wconversion -Wsign-conversion -Wfloat-equal -Wcast-align -Wformat=2 -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wdouble-promotion -Wuse-after-free -Wno-null-dereference)
elseif(WIN32)
    add_compile_options(/W4 /WX /permissive-)
endif()

enable_testing()

if(BUILD_CLIENT)
    set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

    add_executable(generate_assets scripts/generate_embedded_assets.cpp)
    target_include_directories(generate_assets PRIVATE ${CMAKE_SOURCE_DIR}/libs/Multimedia/AssetManager)

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp ${CMAKE_BINARY_DIR}/EmbeddedAssets.hpp
        COMMAND generate_assets ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/EmbeddedAssets
        DEPENDS generate_assets
    )

    set_source_files_properties(${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp PROPERTIES GENERATED TRUE)
    set_source_files_properties(${CMAKE_BINARY_DIR}/EmbeddedAssets.hpp PROPERTIES GENERATED TRUE)

    add_custom_target(generate_embedded_assets DEPENDS ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp ${CMAKE_BINARY_DIR}/EmbeddedAssets.hpp)
endif()

add_subdirectory(common)
add_subdirectory(libs)

if(BUILD_CLIENT)
    add_subdirectory(client)
endif()

if(BUILD_SERVER)
    add_subdirectory(server)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
