add_library(DeathSystem SHARED
    DeathSystem.hpp
    DeathSystem.cpp
)

target_link_libraries(DeathSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(DeathSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)
