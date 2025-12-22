add_library(LifetimeSystem SHARED
    LifetimeSystem.hpp
    LifetimeSystem.cpp
)

target_link_libraries(LifetimeSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(LifetimeSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)
