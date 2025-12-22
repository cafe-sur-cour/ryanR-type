add_library(ShootingSystem SHARED
    ShootingSystem.hpp
    ShootingSystem.cpp
)

target_link_libraries(ShootingSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(ShootingSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)
