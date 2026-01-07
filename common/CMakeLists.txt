set(ECS_LIB_NAME ECS)
set(ERROR_LIB ERROR)

# ============================================================================
# Precompiled Headers Configuration
# ============================================================================
set(PCH_HEADERS
    <vector>
    <map>
    <unordered_map>
    <set>
    <unordered_set>
    <string>
    <string_view>
    <memory>
    <functional>
    <optional>
    <variant>
    <array>
    <tuple>
    <algorithm>
    <utility>
    <type_traits>
    <chrono>
    <cstdint>
    <cstddef>
    <cassert>
    <iostream>
    <fstream>
    <sstream>
    <stdexcept>
)

if(USE_PCH)
    message(STATUS "Precompiled headers enabled")
endif()

add_subdirectory(DLLoader)
add_subdirectory(types)
add_subdirectory(ECS)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(Error)
add_subdirectory(Signal)
add_subdirectory(Prefab)
add_subdirectory(resourceManager)
add_subdirectory(InputMapping)
add_subdirectory(Parser)
add_subdirectory(CollisionRules)
add_subdirectory(SpatialGrid)
add_subdirectory(packet)

set(DEBUG_LIB_NAME DEBUG)

add_library(${DEBUG_LIB_NAME} STATIC)

target_include_directories(${DEBUG_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${DEBUG_LIB_NAME} PUBLIC
    debug.cpp
    debug.hpp
)

if(USE_PCH)
    target_precompile_headers(${DEBUG_LIB_NAME} PRIVATE ${PCH_HEADERS})
endif()
