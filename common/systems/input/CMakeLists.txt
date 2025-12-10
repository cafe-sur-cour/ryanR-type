add_library(InputNormalizer STATIC
    InputNormalizer.cpp
)

target_include_directories(InputNormalizer PRIVATE
    ${CMAKE_SOURCE_DIR}/common
)