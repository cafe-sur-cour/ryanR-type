add_library(InputNormalizer STATIC
    InputNormalizer.cpp
)

set_target_properties(InputNormalizer PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(InputNormalizer PRIVATE
    ${CMAKE_SOURCE_DIR}/common
)