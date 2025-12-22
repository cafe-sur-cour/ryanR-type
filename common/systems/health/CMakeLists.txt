add_library(HealthSystem SHARED
    HealthSystem.hpp
    HealthSystem.cpp
)

target_link_libraries(HealthSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(HealthSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)
