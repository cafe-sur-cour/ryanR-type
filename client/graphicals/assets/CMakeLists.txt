set(ASSETS_LIB_NAME Assets)

add_library(${ASSETS_LIB_NAME}
    AssetManager.cpp
    AssetManager.hpp
)

target_include_directories(${ASSETS_LIB_NAME}
    PUBLIC
        ${CMAKE_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)
