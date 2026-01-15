cmake_minimum_required(VERSION 3.23)

project(r-type)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# Precompiled Headers Configuration
# ============================================================================
option(USE_PCH "Use precompiled headers to speed up compilation" ON)

# ============================================================================
# Unity Build Configuration
# ============================================================================
option(USE_UNITY_BUILD "Use Unity Build to speed up compilation" ON)
if(USE_UNITY_BUILD)
    # Don't enable globally - we'll enable it per-target to avoid static library conflicts
    set(CMAKE_UNITY_BUILD OFF)
    # Reduce batch size in CI to save memory
    if(DEFINED ENV{CI} OR DEFINED ENV{GITHUB_ACTIONS})
        set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)
        message(STATUS "CI detected: Unity Build batch size reduced to ${CMAKE_UNITY_BUILD_BATCH_SIZE}")
    else()
        set(CMAKE_UNITY_BUILD_BATCH_SIZE 16)
    endif()
    message(STATUS "Unity Build will be enabled for executables and shared libraries only")
    message(STATUS "Unity Build batch size: ${CMAKE_UNITY_BUILD_BATCH_SIZE}")
endif()

# ============================================================================
# Compiler Cache (ccache/sccache) Configuration
# ============================================================================
find_program(CCACHE_PROGRAM ccache)
find_program(SCCACHE_PROGRAM sccache)

if(SCCACHE_PROGRAM)
    message(STATUS "Using sccache: ${SCCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER ${SCCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${SCCACHE_PROGRAM})
elseif(CCACHE_PROGRAM)
    message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
else()
    message(STATUS "No compiler cache found (ccache/sccache)")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g3 -O0 -fno-omit-frame-pointer)
endif()

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

find_package(nlohmann_json CONFIG REQUIRED)

option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_COVERAGE "Build coverage" OFF)
option(BUILD_CLIENT "Build client and multimedia library" ON)
option(BUILD_SERVER "Build server" ON)

if(APPLE)
    add_compile_options(-Wall -Wextra -Werror -Wshadow -Wpedantic -Wconversion -Wsign-conversion -Wfloat-equal -Wcast-align -Wformat=2 -Wnull-dereference -Wdouble-promotion)
elseif(UNIX)
    add_compile_options(-Wall -Wextra -Werror -Wshadow -Wpedantic -Wno-conversion -Wsign-conversion -Wno-float-equal -Wcast-align -Wformat=2 -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wdouble-promotion -Wuse-after-free -Wno-null-dereference)
elseif(WIN32)
    add_compile_options(/W4 /WX /permissive-)
endif()

if(BUILD_CLIENT)
    set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

    add_executable(generate_assets scripts/generate_embedded_assets.cpp)
    target_include_directories(generate_assets PRIVATE ${CMAKE_SOURCE_DIR}/libs/Multimedia/AssetManager)

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp ${CMAKE_BINARY_DIR}/EmbeddedAssets.hpp ${CMAKE_BINARY_DIR}/EmbeddedAssets_list.txt
        COMMAND generate_assets ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/EmbeddedAssets
        DEPENDS generate_assets
        COMMENT "Generating embedded assets from ${ASSETS_DIR}"
    )

    set_source_files_properties(${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp PROPERTIES GENERATED TRUE)
    set_source_files_properties(${CMAKE_BINARY_DIR}/EmbeddedAssets.hpp PROPERTIES GENERATED TRUE)

    add_custom_target(generate_embedded_assets DEPENDS ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp ${CMAKE_BINARY_DIR}/EmbeddedAssets.hpp ${CMAKE_BINARY_DIR}/EmbeddedAssets_list.txt)
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
    enable_testing()
    add_subdirectory(tests)
endif()

