set(COMPRESSION_LIB_NAME Compression)

find_package(ZLIB REQUIRED)

add_library(${COMPRESSION_LIB_NAME} STATIC
    Compression.hpp
    Compression.cpp
)

# Required for linking static library into shared library
set_target_properties(${COMPRESSION_LIB_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${COMPRESSION_LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${COMPRESSION_LIB_NAME}
    PUBLIC ZLIB::ZLIB
)
