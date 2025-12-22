add_library(SpawnSystem SHARED
    SpawnSystem.hpp
    SpawnSystem.cpp
)

target_link_libraries(SpawnSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(SpawnSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)
