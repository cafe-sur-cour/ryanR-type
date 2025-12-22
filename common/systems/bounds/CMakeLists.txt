add_library(OutOfBoundsSystem SHARED
    OutOfBoundsSystem.hpp
    OutOfBoundsSystem.cpp
)

target_link_libraries(OutOfBoundsSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(OutOfBoundsSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)
